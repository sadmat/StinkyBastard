#include "gameheaderwidget.h"
#include <Wt/WHBoxLayout.h>
#include "scorewidget.h"

namespace Bastard
{

GameHeaderWidget::GameHeaderWidget()
{
    Wt::WHBoxLayout *layout = this->setLayout(std::make_unique<Wt::WHBoxLayout>());
    layout->setSpacing(5);
    layout->addStretch(238);
    _scoreWidget = layout->addWidget(std::make_unique<ScoreWidget>("SCORE"));
    _bestScoreWidget = layout->addWidget(std::make_unique<ScoreWidget>("BEST"));
}

unsigned GameHeaderWidget::bestScore() const
{
    return _bestScoreWidget->score();
}

void GameHeaderWidget::setBestScore(unsigned score)
{
    _bestScoreWidget->setScore(score);
}

unsigned GameHeaderWidget::score() const
{
    return _scoreWidget->score();
}

void GameHeaderWidget::setScore(unsigned score)
{
    _scoreWidget->setScore(score);
}

}


