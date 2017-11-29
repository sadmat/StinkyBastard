#ifndef GAMEBOARDWIDGET_H
#define GAMEBOARDWIDGET_H

#include <Wt/WPaintedWidget.h>
#include <Tile.h>
#include <GameCore.h>
#include <map>
#include "../utils/TilePositionComparer.h"

namespace nn2048
{

using namespace Game2048Core;

typedef std::map<TilePosition, unsigned int, TilePositionComparer> TileMap;
typedef std::map<unsigned int, Wt::WColor> ColorMap;

class GameBoardWidget : public Wt::WPaintedWidget
{
public:
    GameBoardWidget();

    void setInitialTiles(TilePosition tile1, TilePosition tile2);

    void onReset();
    void onTileMoved(TilePosition from, TilePosition to, bool merged);
    void onTileCreated(unsigned int value, TilePosition position);
    void onGameStateChanged(const GameState &state);

protected:
    void paintEvent(Wt::WPaintDevice *device);
    void drawBackground(Wt::WPainter &painter);
    void drawSlots(Wt::WPainter &painter);
    void drawSlot(Wt::WPainter &painter, const Wt::WPointF &position);
    void drawTiles(Wt::WPainter &painter);
    void drawTile(Wt::WPainter &painter, const Wt::WPointF &position, unsigned int value);

    static void initializeColorMap();
    const Wt::WColor &tileColor(int value) const;
    const Wt::WColor &fontColor(int value) const;

private:
     TileMap _tileMap;
     static ColorMap _colorMap;
     static Wt::WColor _transparentColor;
     static Wt::WColor _bigValueColor;
     static Wt::WColor _darkFontColor;
     static Wt::WColor _lightFontColor;
     static Wt::WFont _font;
};

}

#endif // GAMEBOARDWIDGET_H
