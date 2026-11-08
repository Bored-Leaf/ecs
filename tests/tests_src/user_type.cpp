#include <utility>

#include "../tests_include/user_type.hpp"

int UserType::constructorMoves{0};
int UserType::constructorCopies{0};
int UserType::assignmentMoves{0};
int UserType::assignmentCopies{0};

UserType::UserType(std::string str) : m_str(std::move(str)) {}

UserType::UserType(const UserType& other) : m_str(other.m_str) {
    constructorCopies++;
}

UserType::UserType(UserType&& other) noexcept : m_str(std::move(other.m_str)) {
    constructorMoves++;
}

UserType& UserType::operator=(const UserType& other) {
    if (this == &other) {
        return *this;
    }

    m_str = other.m_str;

    assignmentCopies++;

    return *this;
}

UserType& UserType::operator=(UserType&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    m_str = std::move(other.m_str);

    assignmentMoves++;

    return *this;
}

int UserType::getConstructorMoves() {
    return constructorMoves;
}

int UserType::getConstructorCopies() {
    return constructorCopies;
}

int UserType::getAssignmentMoves() {
    return assignmentMoves;
}

int UserType::getAssignmentCopies() {
    return assignmentCopies;
}

void UserType::resetConstructorMoves() {
    constructorMoves = 0;
}

void UserType::resetConstructorCopies() {
    constructorCopies = 0;
}

void UserType::resetAssignmentMoves() {
    assignmentMoves = 0;
}

void UserType::resetAssignmentCopies() {
    assignmentCopies = 0;
}

void UserType::resetAllCopyMoves() {
    constructorMoves = 0;
    constructorCopies = 0;
    assignmentMoves = 0;
    assignmentCopies = 0;
}
