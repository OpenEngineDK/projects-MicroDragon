#include "DragonText.h"
#include <cairo.h>
#include <Logging/Logger.h>

DragonText::DragonText(CairoSurfaceResource &res,string t) 
    : text(t),resource(res) {
    Draw();
}

void DragonText::SetString(string t) {
    text = t;
    Draw();
}

void DragonText::Draw() {
    cairo_t* cr = resource.GetContext();

    
    cairo_move_to(cr, 0,0);

    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_set_source_rgba (cr, 0,0,0,0);
    cairo_paint (cr);

    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

    cairo_text_extents_t te;
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.5,.6);
    cairo_select_font_face (cr, "Monaco",
			    CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 55);
    cairo_text_extents (cr, text.c_str(), &te);
    cairo_move_to (cr,  te.width / 2 - te.x_bearing,
		    te.height / 2 - te.y_bearing);
    cairo_show_text (cr, text.c_str());




    resource.Rebind();
}

