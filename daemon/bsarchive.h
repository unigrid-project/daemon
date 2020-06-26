// Copyright (c) 2017-2019 The Swipp developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING.daemon or http://www.opensource.org/licenses/mit-license.php.

#include <functional>
#include <cstdio>

class BSArchive
{
private:
    std::FILE *file;
    unsigned char *in;
    unsigned char *out;
    std::function<void(double percentage)> progress;

public:
    BSArchive(std::FILE *file, std::function<void(double percentage)> progress = [](double percentage) -> void { });
    ~BSArchive();

    bool verifyHash();
    int unarchive(std::FILE *destination);
};
