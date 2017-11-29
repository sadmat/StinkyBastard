#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>

namespace nn2048
{

class ScoreWidget: public Wt::WContainerWidget
{
public:
    ScoreWidget();
    ScoreWidget(const std::string &name, unsigned score = 0);

    std::string name() const { return _nameLabel->text().toUTF8(); }
    void setName(const std::string &name) { _nameLabel->setText(name); }

    unsigned score() const { return _score; }
    void setScore(unsigned score);

private:
    unsigned _score;
    Wt::WText *_nameLabel;
    Wt::WText *_scoreLabel;
};

}

#endif // SCOREWIDGET_H
