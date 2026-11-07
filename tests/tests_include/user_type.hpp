#pragma once

#include <string>

struct UserType {
    static int moves;
    static int copies;

    std::string m_str{};

    UserType(std::string str);
    UserType(const UserType& userType);
    UserType(UserType&& userType) noexcept;

    UserType& operator=(const UserType& other);
    UserType& operator=(UserType&& other) noexcept;

    static int getMoves();

    static int getCopies();

    static void resetMoves();

    static void resetCopies();
};
