//
// Created by mateu on 15.03.2021.
//

#ifndef OPTYMALIZACJA_STD_EXTENSIONS_H
#define OPTYMALIZACJA_STD_EXTENSIONS_H
namespace std
{
    template<>
    struct hash<std::pair<int, int>>
    {
        size_t operator () (std::pair<int, int> const& p) const
        {
            return (std::hash<int>()(p.first) ^ std::hash<int>()(p.second));
        }
    };
}
#endif //OPTYMALIZACJA_STD_EXTENSIONS_H
