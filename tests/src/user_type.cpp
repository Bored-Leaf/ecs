#include <utility>

#include "../tests_include/user_type.hpp"

int UserType::copies{0};
int UserType::moves{0};

UserType::UserType(std::string str) : m_str(std::move(str)) {}

UserType::UserType(const UserType& other) : m_str(other.m_str) {
    copies++;
}

UserType::UserType(UserType&& other) noexcept : m_str(std::move(other.m_str)) {
    moves++;
}

UserType& UserType::operator=(const UserType& other) {
    if (this == &other) {
        return *this;
    }

    m_str = other.m_str;

    return *this;
}

UserType& UserType::operator=(UserType&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    m_str = std::move(other.m_str);

    return *this;
}

int UserType::getMoves() {
    return moves;
}

int UserType::getCopies() {
    return copies;
}

void UserType::resetMoves() {
    moves = 0;
}

void UserType::resetCopies() {
    copies = 0;
}
