#include "gamewidget.h"
#include <Wt/WVBoxLayout.h>
#include "gameheaderwidget.h"
#include "gameboardwidget.h"

namespace Bastard
{

GameWidget::GameWidget()
{
    setWidth(500);
    setContentAlignment(Wt::AlignmentFlag::Center);
    Wt::WVBoxLayout *layout = this->setLayout(std::make_unique<Wt::WVBoxLayout>());
    _headerWidget = layout->addWidget(std::make_unique<GameHeaderWidget>());
    _boardWidget = layout->addWidget(std::make_unique<GameBoardWidget>());
}

}


