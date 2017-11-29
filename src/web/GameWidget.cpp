#include "GameWidget.h"
#include <Wt/WVBoxLayout.h>
#include "GameHeaderWidget.h"
#include "GameBoardWidget.h"

namespace nn2048
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


