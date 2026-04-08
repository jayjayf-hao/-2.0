#include "persistence/SaveSystem.h"
#include <fstream>
#include <set>
#include <sstream>

namespace {

bool parseInt(const std::string& text, int& outValue) {
    try {
        size_t parsed = 0;
        int value = std::stoi(text, &parsed);
        if (parsed != text.size()) {
            return false;
        }
        outValue = value;
        return true;
    } catch (...) {
        return false;
    }
}

bool parsePoint(const std::string& text, Point& outPoint) {
    const size_t comma = text.find(',');
    if (comma == std::string::npos) {
        return false;
    }
    int x = 0;
    int y = 0;
    if (!parseInt(text.substr(0, comma), x)) {
        return false;
    }
    if (!parseInt(text.substr(comma + 1), y)) {
        return false;
    }
    outPoint = Point(x, y);
    return true;
}

bool inRange(int value, int min, int max) {
    return value >= min && value <= max;
}

} // namespace

bool SaveSystem::saveGame(const std::string& filename, const SaveData& data) {
    std::ofstream out(filename, std::ios::trunc);
    if (!out) {
        return false;
    }

    out << "score=" << data.score << "\n";
    out << "direction=" << static_cast<int>(data.direction) << "\n";
    out << "speed=" << static_cast<int>(data.speed) << "\n";
    out << "state=" << static_cast<int>(data.state) << "\n";
    out << "food=" << data.food.x << "," << data.food.y << "\n";
    out << "snake=";

    for (size_t i = 0; i < data.snakeBody.size(); ++i) {
        out << data.snakeBody[i].x << "," << data.snakeBody[i].y;
        if (i + 1 < data.snakeBody.size()) {
            out << ";";
        }
    }
    out << "\n";

    return true;
}

bool SaveSystem::loadGame(const std::string& filename, int width, int height, SaveData& outData, std::string& error) {
    std::ifstream in(filename);
    if (!in) {
        error = "save file not found";
        return false;
    }

    SaveData temp;
    bool hasSnake = false;
    bool hasFood = false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        const size_t sep = line.find('=');
        if (sep == std::string::npos) {
            error = "malformed line";
            return false;
        }

        const std::string key = line.substr(0, sep);
        const std::string value = line.substr(sep + 1);

        if (key == "score") {
            if (!parseInt(value, temp.score) || temp.score < 0) {
                error = "invalid score";
                return false;
            }
        } else if (key == "direction") {
            int raw = 0;
            if (!parseInt(value, raw) || !inRange(raw, static_cast<int>(Direction::Stop), static_cast<int>(Direction::Down))) {
                error = "invalid direction";
                return false;
            }
            temp.direction = static_cast<Direction>(raw);
        } else if (key == "speed") {
            int raw = 0;
            if (!parseInt(value, raw) || !inRange(raw, static_cast<int>(SpeedLevel::Low), static_cast<int>(SpeedLevel::High))) {
                error = "invalid speed";
                return false;
            }
            temp.speed = static_cast<SpeedLevel>(raw);
        } else if (key == "state") {
            int raw = 0;
            if (!parseInt(value, raw) || !inRange(raw, static_cast<int>(GameState::Menu), static_cast<int>(GameState::Exit))) {
                error = "invalid state";
                return false;
            }
            temp.state = static_cast<GameState>(raw);
        } else if (key == "food") {
            Point p;
            if (!parsePoint(value, p) || p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
                error = "invalid food position";
                return false;
            }
            temp.food = p;
            hasFood = true;
        } else if (key == "snake") {
            temp.snakeBody.clear();
            std::set<long long> visited;
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ';')) {
                Point p;
                if (!parsePoint(token, p)) {
                    error = "invalid snake point";
                    return false;
                }
                if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
                    error = "snake point out of range";
                    return false;
                }
                const long long keyPoint = static_cast<long long>(p.y) * 10000LL + p.x;
                if (visited.count(keyPoint) > 0) {
                    error = "duplicated snake point in save";
                    return false;
                }
                visited.insert(keyPoint);
                temp.snakeBody.push_back(p);
            }
            if (temp.snakeBody.empty()) {
                error = "snake body empty";
                return false;
            }
            hasSnake = true;
        }
    }

    if (!hasSnake || !hasFood) {
        error = "missing required save fields";
        return false;
    }

    outData = temp;
    return true;
}
