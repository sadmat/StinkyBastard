#include "GameBoardWidget.h"
#include <Wt/WPainter.h>
#include <GameCore.h>

#define BOARD_SIZE 500
#define TILE_SIZE 107
#define TILES_PER_ROW 4
#define TILE_ROWS 4
#define BORDER_SIZE 14
#define INITIAL_TILE_VALUE 2
#define FONT_SIZE 45

namespace nn2048
{

ColorMap GameBoardWidget::_colorMap = ColorMap();
Wt::WColor GameBoardWidget::_transparentColor = Wt::WColor(Wt::StandardColor::Transparent);
Wt::WColor GameBoardWidget::_bigValueColor = Wt::WColor(60, 58, 50);
Wt::WColor GameBoardWidget::_darkFontColor = Wt::WColor(119, 110, 101);
Wt::WColor GameBoardWidget::_lightFontColor = Wt::WColor(249, 246, 242);
Wt::WFont GameBoardWidget::_font = Wt::WFont(Wt::FontFamily::SansSerif);

GameBoardWidget::GameBoardWidget()
{
    resize(BOARD_SIZE, BOARD_SIZE);
    if (_colorMap.size() == 0)
        initializeColorMap();
    if (_font.sizeLength().value() != FONT_SIZE)
        _font.setSize(Wt::WLength(FONT_SIZE));
}

void GameBoardWidget::setInitialTiles(TilePosition tile1, TilePosition tile2)
{
    _tileMap[tile1] = INITIAL_TILE_VALUE;
    _tileMap[tile2] = INITIAL_TILE_VALUE;
    update();
}

void GameBoardWidget::onReset()
{
    _tileMap.clear();
}

void GameBoardWidget::onTileMoved(TilePosition from, TilePosition to, bool merged)
{
    // TODO: add animation
    unsigned int value = _tileMap[from];
    _tileMap.erase(from);
    if (!merged)
        _tileMap[to] = value;
    else _tileMap[to] += value;
    update();
}

void GameBoardWidget::onTileCreated(unsigned int value, TilePosition position)
{
    // TODO: add animation
    _tileMap[position] = value;
    update();
}

void GameBoardWidget::onGameStateChanged(const GameState &state)
{
    _tileMap.clear();
    for (unsigned int i = 0; i < state.board.size(); ++i)
    {
        for (unsigned int j = 0; j < state.board.size(); ++j)
        {
            if (!state.board[i][j].isEmpty())
                _tileMap[{i, j}] = state.board[i][j].value();
        }
    }
    update();
}

void GameBoardWidget::paintEvent(Wt::WPaintDevice *device)
{
    Wt::WPainter painter(device);
    drawBackground(painter);
    drawSlots(painter);
    drawTiles(painter);
}

void GameBoardWidget::drawBackground(Wt::WPainter &painter)
{
    // TODO: rounded corners
    Wt::WColor color(187, 173, 160);
    painter.fillRect(0.0, 0.0, BOARD_SIZE, BOARD_SIZE, color);
}

void GameBoardWidget::drawSlots(Wt::WPainter &painter)
{
    // TODO: rounded corners
    Wt::WColor color(205, 193, 180);
    for (int i = 0; i < TILES_PER_ROW; ++i)
    {
        double x = BORDER_SIZE + (TILE_SIZE + BORDER_SIZE) * i;
        for (int j = 0; j < TILE_ROWS; ++j)
        {
            double y = BORDER_SIZE + (TILE_SIZE + BORDER_SIZE) * j;
            painter.fillRect(x, y, TILE_SIZE, TILE_SIZE, color);
        }
    }
}

void GameBoardWidget::drawTiles(Wt::WPainter &painter)
{
    for (const auto &tile: _tileMap)
    {
        const TilePosition &pos = tile.first;
        unsigned int value = tile.second;
        double x = BORDER_SIZE + (TILE_SIZE + BORDER_SIZE) * pos.column;
        double y = BORDER_SIZE + (TILE_SIZE + BORDER_SIZE) * pos.row;
        drawTile(painter, Wt::WPointF(x, y), value);
    }
}

void GameBoardWidget::drawTile(Wt::WPainter &painter, const Wt::WPointF &position, unsigned int value)
{
    painter.translate(position.x(), position.y());

    Wt::WRectF rect(0.0, 0.0, TILE_SIZE, TILE_SIZE);

    // Background
    const Wt::WColor &backgroundColor = tileColor(value);
    painter.fillRect(rect, backgroundColor);

    // Value
    painter.setPen(fontColor(value));
    painter.setFont(_font);
    painter.drawText(rect, Wt::AlignmentFlag::Center | Wt::AlignmentFlag::Middle, Wt::WString("{1}").arg(value));

    painter.translate(-position.x(), -position.y());
}

void GameBoardWidget::initializeColorMap()
{
    _colorMap[2] = Wt::WColor(238, 228, 218);
    _colorMap[4] = Wt::WColor(237, 224, 200);
    _colorMap[8] = Wt::WColor(242, 177, 121);
    _colorMap[16] = Wt::WColor(245, 149, 99);
    _colorMap[32] = Wt::WColor(246, 124, 95);
    _colorMap[64] = Wt::WColor(246, 94, 59);
    _colorMap[128] = Wt::WColor(237, 207, 114);
    _colorMap[256] = Wt::WColor(237, 204, 97);
    _colorMap[512] = Wt::WColor(237, 200, 80);
    _colorMap[1024] = Wt::WColor(237, 197, 63);
    _colorMap[2048] = Wt::WColor(237, 194, 46);
}

const Wt::WColor &GameBoardWidget::tileColor(int value) const
{
    if (_colorMap.count(value) > 0)
        return _colorMap[value];
    else if (value < 2)
        return _transparentColor;
    return _bigValueColor;
}

const Wt::WColor &GameBoardWidget::fontColor(int value) const
{
    if (value <= 4)
        return _darkFontColor;
    return _lightFontColor;
}

}


