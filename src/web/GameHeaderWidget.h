#ifndef GAMEHEADERWIDGET_H
#define GAMEHEADERWIDGET_H

#include <Wt/WContainerWidget.h>

namespace nn2048
{

class ScoreWidget;

class GameHeaderWidget : public Wt::WContainerWidget
{
public:
    GameHeaderWidget();

    unsigned bestScore() const;
    void setBestScore(unsigned score);

    unsigned score() const;
    void setScore(unsigned score);

private:
    ScoreWidget *_scoreWidget;
    ScoreWidget *_bestScoreWidget;
};

}

#endif // GAMEHEADERWIDGET_H
