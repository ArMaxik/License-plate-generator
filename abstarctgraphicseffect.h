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

class BaseGraphicsEffect : public AbstarctGraphicsEffect
{
public:
    BaseGraphicsEffect() {}
    ~BaseGraphicsEffect() override {}

    QImage apply(QImage *img) override;
};

class BaseDecoratorGraphicsEffect : public AbstarctGraphicsEffect
{
public:
    BaseDecoratorGraphicsEffect(AbstarctGraphicsEffect *wrap = nullptr)
        : effect(wrap) {}
    ~BaseDecoratorGraphicsEffect() override {}

    QImage apply(QImage *img) override = 0;

protected:
    AbstarctGraphicsEffect *effect;
};

class NormalHeightGraphicsEffect : public BaseDecoratorGraphicsEffect
{
public:
    NormalHeightGraphicsEffect(AbstarctGraphicsEffect *wrap = nullptr)
        : BaseDecoratorGraphicsEffect(wrap) {}
    ~NormalHeightGraphicsEffect() override {}

    virtual QImage apply(QImage *img) override;

};

#endif // ABSTARCTGRAPHICSEFFECT_H
