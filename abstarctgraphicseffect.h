#ifndef ABSTARCTGRAPHICSEFFECT_H
#define ABSTARCTGRAPHICSEFFECT_H

#include "nodes.h"


class AbstarctGraphicsEffect
{
public:
    AbstarctGraphicsEffect() {}
    virtual ~AbstarctGraphicsEffect() {}

    virtual QImage apply(QImage *img) = 0;
};

class BasicGraphicsEffect : public AbstarctGraphicsEffect
{
public:
    BasicGraphicsEffect() {}
    ~BasicGraphicsEffect() override {}

    QImage apply(QImage *img) override;
};

class BasicDecoratorGraphicsEffect : public AbstarctGraphicsEffect
{
public:
    BasicDecoratorGraphicsEffect(AbstarctGraphicsEffect *wrap = nullptr);
    ~BasicDecoratorGraphicsEffect() override { delete effect; }

    QImage apply(QImage *img) override = 0;

protected:
    AbstarctGraphicsEffect *effect;
};

class NormalHeightGraphicsEffect : public BasicDecoratorGraphicsEffect
{
public:
    NormalHeightGraphicsEffect(AbstarctGraphicsEffect *wrap = nullptr)
        : BasicDecoratorGraphicsEffect(wrap) {}
    ~NormalHeightGraphicsEffect() override {}

    virtual QImage apply(QImage *img) override;

};

#endif // ABSTARCTGRAPHICSEFFECT_H
