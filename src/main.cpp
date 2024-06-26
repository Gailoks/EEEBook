#include <Arduino.h>
#include "gui/elements/Grid.h"
#include <GxEPD2_BW.h>
#include <gdey/GxEPD2_420_GDEY042T81.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include "gui/elements/Rectangle.h"
#include "gui/GraphicsEngine.h"

GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> display(GxEPD2_420_GDEY042T81(/*CS=5*/ SS, /*DC=*/ 4, /*RST=*/ 16, /*BUSY=*/ 17)); // GYE042A87, 400x300, SSD1683 (HINK-E042-A07-FPC-A1)
Grid grid;


class GxEPD_GraphicsEngine : public GraphicsEngine_
{
    void draw_rectangle(Bounds bounds) const override
    {
        Point start = bounds.start_point;
        Size size = bounds.size;
        display.setPartialWindow(start.x, start.y, size.width, size.height);
        display.firstPage();
        do
        {
            display.fillScreen(GxEPD_WHITE);
            display.fillRect(start.x, start.y, size.width, size.height,GxEPD_BLACK);
        }
        while (display.nextPage());
    }

    void print_text(Point start_point, char* text) const override
    {
        display.setFont(&FreeMonoBold12pt7b);
        display.setTextColor(GxEPD_BLACK);
        int16_t tbx, tby; uint16_t tbw, tbh;
        display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
        // center bounding box by transposition of origin:
        uint16_t x = ((display.width() - tbw) / 2) - tbx;
        uint16_t y = ((display.height() - tbh) / 2) - tby;
        display.firstPage();
        do
        {
            display.fillScreen(GxEPD_WHITE);
            display.setCursor(x, y);
            display.print(text);
        }
        while (display.nextPage());
    }
};



void setup()
{
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);

    GxEPD_GraphicsEngine engine;

    display.init(9600, true, 2, false);
    display.setRotation(1);

    GFX root_gfx(&engine, create_size(300, 400));
    

    Rectangle rect1 = new Rectangle_(5, 5);
    Rectangle rect2 = new Rectangle_(5, 5);
    Rectangle rect3 = new Rectangle_(5, 5);
    Rectangle rect4 = new Rectangle_(5, 5);

    grid = new Grid_(
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1) },
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1) },
        std::vector<GridElement> { fit_in_grid(rect1, 0, 0), fit_in_grid(rect2, 0, 1), fit_in_grid(rect3, 1, 0), fit_in_grid(rect4, 1, 1) }
    );

    grid->render(root_gfx);

    Serial.println(8);

    //delete rect1, rect2, rect3, rect4, grid;
}

void loop()
{

}
