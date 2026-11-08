#pragma once

#include <string>

struct UserType {
    static int constructorMoves;
    static int constructorCopies;
    static int assignmentMoves;
    static int assignmentCopies;

    std::string m_str{};

    UserType(std::string str);
    UserType(const UserType& userType);
    UserType(UserType&& userType) noexcept;

    UserType& operator=(const UserType& other);
    UserType& operator=(UserType&& other) noexcept;

    static int getConstructorMoves();
    static int getConstructorCopies();
    static int getAssignmentMoves();
    static int getAssignmentCopies();

    static void resetConstructorMoves();
    static void resetConstructorCopies();
    static void resetAssignmentMoves();
    static void resetAssignmentCopies();
    static void resetAllCopyMoves();
};
