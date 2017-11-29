#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <Wt/WContainerWidget.h>

namespace nn2048
{

class GameHeaderWidget;
class GameBoardWidget;

class GameWidget : public Wt::WContainerWidget
{
public:
    GameWidget();

    GameHeaderWidget *headerWidget() { return _headerWidget; }
    GameBoardWidget *boardWidget() { return _boardWidget; }

private:
    GameHeaderWidget *_headerWidget;
    GameBoardWidget *_boardWidget;
};

}

#endif // GAMEWIDGET_H
