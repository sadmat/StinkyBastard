#include "ScoreWidget.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WCssDecorationStyle.h>

namespace nn2048
{

ScoreWidget::ScoreWidget():
    ScoreWidget("SCORE", 0)
{}

ScoreWidget::ScoreWidget(const std::string &name, unsigned score):
    _score(score)
{
    this->resize(126, 55);
    this->decorationStyle().setBackgroundColor(Wt::WColor(187, 173, 160));
    this->decorationStyle().font().setFamily(Wt::FontFamily::SansSerif);
    this->decorationStyle().font().setWeight(Wt::FontWeight::Bold);

    Wt::WVBoxLayout *layout = this->setLayout(std::make_unique<Wt::WVBoxLayout>());
    _nameLabel = layout->addWidget(std::make_unique<Wt::WText>(name),
                                   0, Wt::AlignmentFlag::Center);
    _nameLabel->decorationStyle().setForegroundColor(Wt::WColor(238, 228, 218));
    _nameLabel->decorationStyle().font().setSize(13);

    _scoreLabel = layout->addWidget(std::make_unique<Wt::WText>(Wt::WString("{1}").arg(score)),
                                    0, Wt::AlignmentFlag::Center);
    _scoreLabel->decorationStyle().setForegroundColor(Wt::WColor(Wt::StandardColor::White));
    _scoreLabel->decorationStyle().font().setSize(24);
}

void ScoreWidget::setScore(unsigned score)
{
    _score = score;
    _scoreLabel->setText(Wt::WString("{1}").arg(score));
}

}


