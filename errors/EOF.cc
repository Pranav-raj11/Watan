export module EndOfFile;

import <stdexcept>;

export class EndOfFileException: public std::exception {
    public:
        const char* what() const noexcept override {
            return "EOF";
        }
};
