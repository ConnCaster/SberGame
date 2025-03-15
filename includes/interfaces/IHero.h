#ifndef SBERGAME_IHERO_H
#define SBERGAME_IHERO_H

class IHero {
public:
    virtual ~IHero() = default;
private:
    unsigned int health_;
    unsigned int protection_;
    unsigned int damage_;
};


#endif
