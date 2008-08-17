#include "DragonText.h"
#include <cairo.h>
#include <Logging/Logger.h>

DragonText::DragonText(CairoSurfaceResource &res,string t) 
    : text(t),resource(res), alignment(LEFT), textsize(50) {
    Draw();
}

void DragonText::SetString(string t) {
    text = t;
    Draw();
}


void DragonText::SetAlignment(Alignment al) {
    alignment = al;
}

void DragonText::SetTextSize(int size) {
    textsize = size;
}


void DragonText::Draw() {
    cairo_t* cr = resource.GetContext();

    int shadowoffset = 3;
    
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_set_source_rgba (cr, 0,0,0,0);
    cairo_paint (cr);

    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0,0.8);

    cairo_select_font_face (cr, "Monaco",
			    CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, textsize);

    cairo_text_extents_t extents;
    cairo_text_extents (cr,
                        text.c_str(),
                        &extents);

    if (alignment == LEFT)
        cairo_move_to(cr, 0,resource.GetHeight()+0.5*extents.y_bearing+shadowoffset);
    else {

        cairo_move_to(cr, resource.GetWidth() - extents.width-extents.x_bearing-shadowoffset, resource.GetHeight()+0.5*extents.y_bearing+shadowoffset);
    }

//     cairo_text_extents (cr, text.c_str(), &te);
//     cairo_move_to (cr,  te.width / 2 - te.x_bearing,
// 		    te.height / 2 - te.y_bearing);
    cairo_show_text (cr, text.c_str());
    
    cairo_rel_move_to(cr,shadowoffset-extents.x_advance,-shadowoffset-extents.y_advance);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.5,.9);
    cairo_show_text (cr, text.c_str());



    resource.Rebind();
}

