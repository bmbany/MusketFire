#pragma once

#define NOCOPY(CLASS)                           \
private:                                        \
CLASS(const CLASS& other) = default;              \
CLASS& operator=(const CLASS& other) = default;    \
CLASS(CLASS&& other) = default;                   \
CLASS& operator=(CLASS&& other) = default;