/*
 *		assetspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "assetspanel.h"
#include "guimanager.h"
#include "style.h"


extern guimanager		g_gui_mgr;


//assetspanel
assetspanel::assetspanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("vertical <weight=20 caption><assets>");

	//TEMP caption
	_caption.bgcolor(CREATOR_WINDOW_CAP_BG);
	_caption.fgcolor(CREATOR_WINDOW_CAP_FG);
	_caption.caption("Assets");
	_place.field("caption") << _caption;


	// sets the images
	auto & img_h_layout = _assets.icon(CTRL_LAYOUT);
	img_h_layout.normal.open("icons/horizontal_layout_dark.png");

	auto & img_btn = _assets.icon(CTRL_BUTTON);
	img_btn.normal.open("icons/button_dark.png");

	auto & img_lbl = _assets.icon(CTRL_LABEL);
	img_lbl.normal.open("icons/label_dark.png");

	auto & img_lst = _assets.icon(CTRL_LISTBOX);
	img_lst.normal.open("icons/listbox_dark.png");

	auto & img_txt = _assets.icon(CTRL_TEXTBOX);
	img_txt.normal.open("icons/textbox_dark.png");

	auto & img_pnl = _assets.icon(CTRL_PANEL);
	img_pnl.normal.open("icons/panel_dark.png");

	auto & img_com = _assets.icon(CTRL_COMBOX);
	img_com.normal.open("icons/combox_dark.png");

	auto & img_spn = _assets.icon(CTRL_SPINBOX);
	img_spn.normal.open("icons/spinbox_dark.png");

	auto & img_chk = _assets.icon(CTRL_CHECKBOX);
	img_chk.normal.open("icons/checkbox_dark.png");

	auto & img_dtc = _assets.icon(CTRL_DATECHOOSER);
	img_dtc.normal.open("icons/datechooser_dark.png");

	auto & img_tlb = _assets.icon(CTRL_TOOLBAR);
	img_tlb.normal.open("icons/toolbar_dark.png");

	auto & img_frm = _assets.icon(CTRL_FORM);
	img_frm.normal.open("icons/form_dark.png");


	// events
	_assets.events().selected([this](const nana::arg_treebox & arg)
	{
		if(arg.operated)
		{
			g_gui_mgr.cursor(arg.item.value<cursor_state>());
		}
	});


	// populate
	auto set = _assets.insert("widgets", "Widgets").value(cursor_state{ cursor_action::select });
	set.append("widgets/form", CTRL_FORM_NAME, cursor_state{ cursor_action::add, CTRL_MAINFORM }).icon(CTRL_FORM);
	set.append("widgets/panel", CTRL_PANEL_NAME, cursor_state{ cursor_action::add, CTRL_MAINPANEL }).icon(CTRL_PANEL);
	set.expand(true);
	//
	set = _assets.insert("layouts", "Layouts").value(cursor_state{ cursor_action::select });
	set.append("layouts/hv", CTRL_LAYOUT_NAME, cursor_state{ cursor_action::add, CTRL_LAYOUT }).icon(CTRL_LAYOUT);
	set.expand(true);
	//
	set = _assets.insert("controls", "Controls").value(cursor_state{ cursor_action::select });
	set.append("controls/button", CTRL_BUTTON_NAME, cursor_state{ cursor_action::add, CTRL_BUTTON }).icon(CTRL_BUTTON);
	set.append("controls/combox", CTRL_COMBOX_NAME, cursor_state{ cursor_action::add, CTRL_COMBOX }).icon(CTRL_COMBOX);
	set.append("controls/label", CTRL_LABEL_NAME, cursor_state{ cursor_action::add, CTRL_LABEL }).icon(CTRL_LABEL);
	set.append("controls/listbox", CTRL_LISTBOX_NAME, cursor_state{ cursor_action::add, CTRL_LISTBOX }).icon(CTRL_LISTBOX);
	set.append("controls/panel", CTRL_PANEL_NAME, cursor_state{ cursor_action::add, CTRL_PANEL }).icon(CTRL_PANEL);
	set.append("controls/spinbox", CTRL_SPINBOX_NAME, cursor_state{ cursor_action::add, CTRL_SPINBOX }).icon(CTRL_SPINBOX);
	set.append("controls/textbox", CTRL_TEXTBOX_NAME, cursor_state{ cursor_action::add, CTRL_TEXTBOX }).icon(CTRL_TEXTBOX);
	set.append("controls/checkbox", CTRL_CHECKBOX_NAME, cursor_state{ cursor_action::add, CTRL_CHECKBOX }).icon(CTRL_CHECKBOX);
	set.append("controls/datechooser", CTRL_DATECHOOSER_NAME, cursor_state{ cursor_action::add, CTRL_DATECHOOSER }).icon(CTRL_DATECHOOSER);
	set.append("controls/toolbar", CTRL_TOOLBAR_NAME, cursor_state{ cursor_action::add, CTRL_TOOLBAR }).icon(CTRL_TOOLBAR);
	set.expand(true);


	_assets.bgcolor(CREATOR_WINDOW_BG);
	_place.field("assets") << _assets;
	_place.collocate();
}


void assetspanel::deselect()
{
	auto item = _assets.selected();
	if(!item.empty())
		item.select(false);
}
