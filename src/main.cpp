/*
 *      Nana C++ Library - Creator
 *      Author: besh81
 */
#include "config.h"
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "pugixml/pugixml.hpp"
#include "guimanager.h"
#include "imagemanager.h"
#include "filemanager.h"
#include "inifile.h"
#include "codegenerator.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"
#include "statusbar.h"
#include "new_project.h"
#include "style.h"


using namespace nana;


guimanager		g_gui_mgr;	// manage all the gui elements
imagemanager	g_img_mgr;
filemanager		g_file_mgr;	// manage absolute and relative path
inifile			g_inifile;
std::string		prj_name;


class nana_creator : public form
{
public:
	nana_creator()
		: form(API::make_center(1200, 700), appear::decorate<appear::sizable, appear::minimize, appear::maximize>())
	{
		caption(CREATOR_NAME " " CREATOR_VERSION);

		_place.div("vertical <weight=30 toolbar><<weight=20% vertical <assets>|<objects>>|<weight=55% canvas>|<props>><weight=24 margin=3 statusbar>");

		_init_ctrls();

		_place.collocate();
	};

	~nana_creator()
	{
		g_gui_mgr.clear();
	}


	bool load_xml(const std::string& filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename.c_str());

		if(!result)
		{
			std::cout << "XML [" << filename << "] parsed with errors\n";
			std::cout << "Error description: " << result.description() << "\n";
			std::cout << "Error offset: " << result.offset << " (error at [..." << result.offset << "]\n\n";
			return false;
		}

		// read root node
		pugi::xml_node root = doc.child(NODE_ROOT);
		if(root.empty())
		{
			std::cout << "Error missing root node: " << NODE_ROOT << "\n";
			return false;
		}

		// deserialize the XML structure
		return g_gui_mgr.deserialize(&root);
	}


	bool save_xml(const std::string& filename)
	{
		pugi::xml_document doc;

		// append root node
		pugi::xml_node root = doc.append_child(NODE_ROOT);
		root.append_attribute("version") = CREATOR_VERSION;

		g_gui_mgr.serialize(&root);

		//doc.print(std::cout); //TEMP debug
		return doc.save_file(filename.c_str());
	}


	bool generate_cpp()
	{
		std::string path;
		auto tpos = prj_name.find_last_of("\\/");
		if(tpos != prj_name.npos)
			path = prj_name.substr(0, tpos);

		codegenerator cpp;
		cpp.generate(handle(), g_gui_mgr.get_root()->child, path);
		cpp.print(std::cout);
		return true;
	}


private:
	void _init_ctrls()
	{
		// toolbar
		_toolbar.scale(21);
		nana::paint::image _img_new;
		_img_new.open("icons/new_prj.png");
		_toolbar.append("New project", _img_new);
		nana::paint::image _img_load;
		_img_load.open("icons/load_prj.png");
		_toolbar.append("Load project", _img_load);
		nana::paint::image _img_save;
		_img_save.open("icons/save.png");
		_toolbar.append("Save project", _img_save);
		nana::paint::image _img_save_as;
		_img_save_as.open("icons/save_as.png");
		_toolbar.append("Save project as...", _img_save_as);
		_toolbar.separate();
		nana::paint::image _img_out;
		_img_out.open("icons/output_on.png");
		_toolbar.append("Generate C++ code", _img_out);
		_toolbar.separate();
		nana::paint::image _img_del;
		_img_del.open("icons/delete.png");
		_toolbar.append("Delete current selection", _img_del);
		_toolbar.separate();
		nana::paint::image _img_up;
		_img_up.open("icons/up.png");
		_toolbar.append("Move up current selection", _img_up);
		nana::paint::image _img_down;
		_img_down.open("icons/down.png");
		_toolbar.append("Move down current selection", _img_down);
		_toolbar.separate();
		nana::paint::image _img_cut;
		_img_cut.open("icons/cut.png");
		_toolbar.append("Cut current selection", _img_cut);
		nana::paint::image _img_copy;
		_img_copy.open("icons/copy.png");
		_toolbar.append("Copy current selection", _img_copy);
		nana::paint::image _img_paste;
		_img_paste.open("icons/paste.png");
		_toolbar.append("Paste into/after current selection", _img_paste);


		_toolbar.events().selected([this](const nana::arg_toolbar & arg)
		{
			if(arg.button == TB_NEW) // new project
			{
				// confirmation message
				if(prj_name != "")
				{
					nana::msgbox m(*this, "Save project", nana::msgbox::yes_no_cancel);
					m.icon(m.icon_warning);
					m << "Do you want to save the current project?";
					auto ret = m();
					if(ret == nana::msgbox::pick_cancel)
						return;
					else if(ret == nana::msgbox::pick_yes)
						save_xml(prj_name);
				}

				g_gui_mgr.clear();
				prj_name = "";

				new_project dlg(*this);
				dlg.modality();

				if(dlg.return_val() == nana::msgbox::pick_ok)
				{
					prj_name = dlg.get_filename();
					g_gui_mgr.new_project(dlg.get_ctrl_type());
					// crea file di progetto
					save_xml(prj_name);
				}
			}
			else if(arg.button == TB_LOAD) // load project
			{
				// confirmation message
				if(prj_name != "")
				{
					nana::msgbox m(*this, "Save project", nana::msgbox::yes_no_cancel);
					m.icon(m.icon_warning);
					m << "Do you want to save the current project?";
					auto ret = m();
					if(ret == nana::msgbox::pick_cancel)
						return;
					else if(ret == nana::msgbox::pick_yes)
						save_xml(prj_name);
				}

				g_gui_mgr.clear();
				prj_name = "";

				filebox fb(*this, true);
				fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
				fb.add_filter("All Files (*.*)", "*.*");

				fb.init_path(g_inifile.load_project_dir());

				if(fb())
				{
					// save load project folder
					//funzione per estrarre la dir da un path
					auto path = get_dir_path(equalize_path(fb.file()));
					if(path != g_inifile.load_project_dir())
						g_inifile.load_project_dir(path, true);

					prj_name = fb.file();
					load_xml(prj_name);
				}
			}
			else if(arg.button == TB_SAVE) // save project
			{
				if(prj_name == "")
					return;

				save_xml(prj_name);
			}
			else if(arg.button == TB_SAVE_AS) // save project as
			{
				if(prj_name == "")
					return;

				filebox fb(*this, false);
				fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
				fb.init_file(prj_name);

				if(fb())
				{
					prj_name = fb.file();
					save_xml(prj_name);
				}
			}
			else if(arg.button == TB_GENERATE) // generate code
			{
				if(prj_name == "")
					return;

				generate_cpp();
			}
			else if(arg.button == TB_DELETE) // delete current selection
			{
				g_gui_mgr.deleteselected();
			}
			else if(arg.button == TB_UP) // move up current selection
			{
				g_gui_mgr.moveupselected();
			}
			else if(arg.button == TB_DOWN) // move down current selection
			{
				g_gui_mgr.movedownselected();
			}
			else if(arg.button == TB_CUT) // cut current selection
			{
				g_gui_mgr.cutselected();
			}
			else if(arg.button == TB_COPY) // copy current selection
			{
				g_gui_mgr.copyselected();
			}
			else if(arg.button == TB_PASTE) // paste into/after current selection
			{
				g_gui_mgr.pasteselected();
			}
		});
		_place.field("toolbar") << _toolbar;

		// assets
		_place.field("assets") << _assets;

		// objects
		_place.field("objects") << _objects;
		

		// properties
		_place.field("props") << _properties;


		// canvas
		_canvas.bgcolor(CREATOR_MAIN_BG);
		_place.field("canvas") << _canvas;


		// statusbar
		_statusbar.set("Ready");
		_place.field("statusbar") << _statusbar;


		g_gui_mgr.root_wd(*this);
		g_gui_mgr.init(&_properties, &_assets, &_objects, &_canvas, &_toolbar, &_statusbar);
	}


	place				_place{ *this };

	resizablecanvas		_canvas{ *this };

	assetspanel			_assets{ *this };
	objectspanel		_objects{ *this };
	propertiespanel		_properties{ *this };

	toolbar				_toolbar{ *this };

	statusbar			_statusbar{ *this };
};


void main()
{
	// init ctrls images
	g_img_mgr.add(CTRL_FIELD, "icons/horizontal_layout.png");
	g_img_mgr.add(CTRL_GRID, "icons/grid_layout.png");
	g_img_mgr.add(CTRL_BUTTON, "icons/button.png");
	g_img_mgr.add(CTRL_LABEL, "icons/label.png");
	g_img_mgr.add(CTRL_TEXTBOX, "icons/textbox.png");
	g_img_mgr.add(CTRL_LISTBOX, "icons/listbox.png");
	g_img_mgr.add(CTRL_PANEL, "icons/panel.png");
	g_img_mgr.add(CTRL_COMBOX, "icons/combox.png");
	g_img_mgr.add(CTRL_SPINBOX, "icons/spinbox.png");
	g_img_mgr.add(CTRL_CHECKBOX, "icons/checkbox.png");
	g_img_mgr.add(CTRL_DATECHOOSER, "icons/datechooser.png");
	g_img_mgr.add(CTRL_TOOLBAR, "icons/toolbar.png");
	g_img_mgr.add(CTRL_FORM, "icons/form.png");
	g_img_mgr.add(CTRL_CATEGORIZE, "icons/categorize.png");
	g_img_mgr.add(CTRL_GROUP, "icons/group.png");
	g_img_mgr.add(CTRL_MENUBAR, "icons/menubar.png");
	g_img_mgr.add(CTRL_PICTURE, "icons/picture.png");
	g_img_mgr.add(CTRL_PROGRESS, "icons/progress.png");
	g_img_mgr.add(CTRL_SLIDER, "icons/slider.png");
	g_img_mgr.add(CTRL_TABBAR, "icons/tabbar.png");
	g_img_mgr.add(CTRL_TREEBOX, "icons/treebox.png");
	g_img_mgr.add(CTRL_SPACER, "icons/spacer.png");


	nana_creator fm;
	fm.show();
	exec();
}
