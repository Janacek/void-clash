#include "GUICallbacks.h"

#include "GUIEngine.h"
#include "CustomListBoxItem.h"

GUICallbacks::GUICallbacks()
{

	toCEGUIkeys[sf::Keyboard::A] = CEGUI::Key::A;
	toCEGUIkeys[sf::Keyboard::B] = CEGUI::Key::B;
	toCEGUIkeys[sf::Keyboard::C] = CEGUI::Key::C;
	toCEGUIkeys[sf::Keyboard::D] = CEGUI::Key::D;
	toCEGUIkeys[sf::Keyboard::E] = CEGUI::Key::E;
	toCEGUIkeys[sf::Keyboard::F] = CEGUI::Key::F;
	toCEGUIkeys[sf::Keyboard::G] = CEGUI::Key::G;
	toCEGUIkeys[sf::Keyboard::H] = CEGUI::Key::H;
	toCEGUIkeys[sf::Keyboard::I] = CEGUI::Key::I;
	toCEGUIkeys[sf::Keyboard::J] = CEGUI::Key::J;
	toCEGUIkeys[sf::Keyboard::K] = CEGUI::Key::K;
	toCEGUIkeys[sf::Keyboard::L] = CEGUI::Key::L;
	toCEGUIkeys[sf::Keyboard::M] = CEGUI::Key::M;
	toCEGUIkeys[sf::Keyboard::N] = CEGUI::Key::N;
	toCEGUIkeys[sf::Keyboard::O] = CEGUI::Key::O;
	toCEGUIkeys[sf::Keyboard::P] = CEGUI::Key::P;
	toCEGUIkeys[sf::Keyboard::Q] = CEGUI::Key::Q;
	toCEGUIkeys[sf::Keyboard::R] = CEGUI::Key::R;
	toCEGUIkeys[sf::Keyboard::S] = CEGUI::Key::S;
	toCEGUIkeys[sf::Keyboard::T] = CEGUI::Key::T;
	toCEGUIkeys[sf::Keyboard::U] = CEGUI::Key::U;
	toCEGUIkeys[sf::Keyboard::V] = CEGUI::Key::V;
	toCEGUIkeys[sf::Keyboard::W] = CEGUI::Key::W;
	toCEGUIkeys[sf::Keyboard::X] = CEGUI::Key::X;
	toCEGUIkeys[sf::Keyboard::Y] = CEGUI::Key::Y;
	toCEGUIkeys[sf::Keyboard::Z] = CEGUI::Key::Z;
	toCEGUIkeys[sf::Keyboard::Num0] = CEGUI::Key::Zero;
	toCEGUIkeys[sf::Keyboard::Num1] = CEGUI::Key::One;
	toCEGUIkeys[sf::Keyboard::Num2] = CEGUI::Key::Two;
	toCEGUIkeys[sf::Keyboard::Num3] = CEGUI::Key::Three;
	toCEGUIkeys[sf::Keyboard::Num4] = CEGUI::Key::Four;
	toCEGUIkeys[sf::Keyboard::Num5] = CEGUI::Key::Five;
	toCEGUIkeys[sf::Keyboard::Num6] = CEGUI::Key::Six;
	toCEGUIkeys[sf::Keyboard::Num7] = CEGUI::Key::Seven;
	toCEGUIkeys[sf::Keyboard::Num8] = CEGUI::Key::Eight;
	toCEGUIkeys[sf::Keyboard::Num9] = CEGUI::Key::Nine;
	toCEGUIkeys[sf::Keyboard::Escape] = CEGUI::Key::Escape;
	toCEGUIkeys[sf::Keyboard::LControl] = CEGUI::Key::LeftControl;
	toCEGUIkeys[sf::Keyboard::LShift] = CEGUI::Key::LeftShift;
	toCEGUIkeys[sf::Keyboard::LAlt] = CEGUI::Key::LeftAlt;
	toCEGUIkeys[sf::Keyboard::LSystem] = CEGUI::Key::LeftWindows;
	toCEGUIkeys[sf::Keyboard::RControl] = CEGUI::Key::RightControl;
	toCEGUIkeys[sf::Keyboard::RShift] = CEGUI::Key::RightShift;
	toCEGUIkeys[sf::Keyboard::RAlt] = CEGUI::Key::RightAlt;
	toCEGUIkeys[sf::Keyboard::RSystem] = CEGUI::Key::RightWindows;
	//toCEGUIkeys[sf::Keyboard::Menu]		= CEGUI::Key: --Dunno what this is
	toCEGUIkeys[sf::Keyboard::LBracket] = CEGUI::Key::LeftBracket;
	toCEGUIkeys[sf::Keyboard::RBracket] = CEGUI::Key::RightBracket;
	toCEGUIkeys[sf::Keyboard::SemiColon] = CEGUI::Key::Semicolon;
	toCEGUIkeys[sf::Keyboard::Comma] = CEGUI::Key::Comma;
	toCEGUIkeys[sf::Keyboard::Period] = CEGUI::Key::Period;
	toCEGUIkeys[sf::Keyboard::Quote] = CEGUI::Key::Apostrophe;
	toCEGUIkeys[sf::Keyboard::Slash] = CEGUI::Key::Slash;
	toCEGUIkeys[sf::Keyboard::BackSlash] = CEGUI::Key::Backslash;
	toCEGUIkeys[sf::Keyboard::Tilde] = CEGUI::Key::Grave;
	toCEGUIkeys[sf::Keyboard::Equal] = CEGUI::Key::Equals;
	toCEGUIkeys[sf::Keyboard::Dash] = CEGUI::Key::Minus;
	toCEGUIkeys[sf::Keyboard::Space] = CEGUI::Key::Space;
	toCEGUIkeys[sf::Keyboard::Return] = CEGUI::Key::Return;
	toCEGUIkeys[sf::Keyboard::BackSpace] = CEGUI::Key::Backspace;
	toCEGUIkeys[sf::Keyboard::Tab] = CEGUI::Key::Tab;
	toCEGUIkeys[sf::Keyboard::PageUp] = CEGUI::Key::PageUp;
	toCEGUIkeys[sf::Keyboard::PageDown] = CEGUI::Key::PageDown;
	toCEGUIkeys[sf::Keyboard::End] = CEGUI::Key::End;
	toCEGUIkeys[sf::Keyboard::Home] = CEGUI::Key::Home;
	toCEGUIkeys[sf::Keyboard::Insert] = CEGUI::Key::Insert;
	toCEGUIkeys[sf::Keyboard::Delete] = CEGUI::Key::Delete;
	toCEGUIkeys[sf::Keyboard::Add] = CEGUI::Key::Add;
	toCEGUIkeys[sf::Keyboard::Subtract] = CEGUI::Key::Subtract;
	toCEGUIkeys[sf::Keyboard::Multiply] = CEGUI::Key::Multiply;
	toCEGUIkeys[sf::Keyboard::Divide] = CEGUI::Key::Divide;
	toCEGUIkeys[sf::Keyboard::Left] = CEGUI::Key::ArrowLeft;
	toCEGUIkeys[sf::Keyboard::Right] = CEGUI::Key::ArrowRight;
	toCEGUIkeys[sf::Keyboard::Up] = CEGUI::Key::ArrowUp;
	toCEGUIkeys[sf::Keyboard::Down] = CEGUI::Key::ArrowDown;
	toCEGUIkeys[sf::Keyboard::Numpad0] = CEGUI::Key::Numpad0;
	toCEGUIkeys[sf::Keyboard::Numpad1] = CEGUI::Key::Numpad1;
	toCEGUIkeys[sf::Keyboard::Numpad2] = CEGUI::Key::Numpad2;
	toCEGUIkeys[sf::Keyboard::Numpad3] = CEGUI::Key::Numpad3;
	toCEGUIkeys[sf::Keyboard::Numpad4] = CEGUI::Key::Numpad4;
	toCEGUIkeys[sf::Keyboard::Numpad5] = CEGUI::Key::Numpad5;
	toCEGUIkeys[sf::Keyboard::Numpad6] = CEGUI::Key::Numpad6;
	toCEGUIkeys[sf::Keyboard::Numpad7] = CEGUI::Key::Numpad7;
	toCEGUIkeys[sf::Keyboard::Numpad8] = CEGUI::Key::Numpad8;
	toCEGUIkeys[sf::Keyboard::Numpad9] = CEGUI::Key::Numpad9;
	toCEGUIkeys[sf::Keyboard::F1] = CEGUI::Key::F1;
	toCEGUIkeys[sf::Keyboard::F2] = CEGUI::Key::F2;
	toCEGUIkeys[sf::Keyboard::F3] = CEGUI::Key::F3;
	toCEGUIkeys[sf::Keyboard::F4] = CEGUI::Key::F4;
	toCEGUIkeys[sf::Keyboard::F5] = CEGUI::Key::F5;
	toCEGUIkeys[sf::Keyboard::F6] = CEGUI::Key::F6;
	toCEGUIkeys[sf::Keyboard::F7] = CEGUI::Key::F7;
	toCEGUIkeys[sf::Keyboard::F8] = CEGUI::Key::F8;
	toCEGUIkeys[sf::Keyboard::F9] = CEGUI::Key::F9;
	toCEGUIkeys[sf::Keyboard::F10] = CEGUI::Key::F10;
	toCEGUIkeys[sf::Keyboard::F11] = CEGUI::Key::F11;
	toCEGUIkeys[sf::Keyboard::F12] = CEGUI::Key::F12;
	toCEGUIkeys[sf::Keyboard::F13] = CEGUI::Key::F13;
	toCEGUIkeys[sf::Keyboard::F14] = CEGUI::Key::F14;
	toCEGUIkeys[sf::Keyboard::F15] = CEGUI::Key::F15;
	toCEGUIkeys[sf::Keyboard::Pause] = CEGUI::Key::Pause;

	//toCEGUIkeys[sf::Mouse::Left]		= CEGUI::LeftButton;
	//toCEGUIkeys[sf::Mouse::Right]		= CEGUI::RightButton;
	//toCEGUIkeys[sf::Mouse::Middle]		= CEGUI::MiddleButton;
	//toCEGUIkeys[sf::Mouse::XButton1]	= CEGUI::X1Button;
	//toCEGUIkeys[sf::Mouse::XButton2]	= CEGUI::X2Button;

	multiSelectionEnabled = false;
}

GUICallbacks::~GUICallbacks()
{

}

bool GUICallbacks::PrintHello(const CEGUI::EventArgs& /*e*/)
{
	return true;
};

bool GUICallbacks::WallCreationSelection(const CEGUI::EventArgs &)
{
	m_app->_objectToCreate = eObjectType::WALL;

	return true;
}

bool GUICallbacks::DeleteObject(const CEGUI::EventArgs &e)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getFirstSelectedItem() == NULL)
		return true;

	int id = box->getFirstSelectedItem()->getID();

	Event::addEvent(ev_DELETE, S_Map->findObjectWithID(id));

	m_app->getMapData()->setModifications(true);
	m_app->getMapData()->setVisibleParameters("00000000000");

	box->removeItem(box->getFirstSelectedItem());

	return true;
}

bool GUICallbacks::ItemSelectionChanged(const CEGUI::EventArgs &e)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	box->setMultiselectEnabled(multiSelectionEnabled);
	if (box->getSelectedCount() == 1)
	{
		m_app->getMapData()->setObjectParameters(box->getFirstSelectedItem()->getID());
	}

	return true;
}

bool GUICallbacks::NewItemSelected(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Combobox *box = static_cast<CEGUI::Combobox *>(myRoot->getChildRecursive("ItemsAvailableCombobox"));

	m_app->CreateNewObject(box->getSelectedItem()->getText().c_str());

	return true;
}

void GUICallbacks::OpenMapThreadFunc()
{
	OPENFILENAME openFileDialog;
	char szFileName[MAX_PATH];
	ZeroMemory(&openFileDialog, sizeof(openFileDialog));
	openFileDialog.lStructSize = sizeof(openFileDialog);
	openFileDialog.hwndOwner = 0;
	openFileDialog.lpstrFilter = "All Files (*.*)\0*.*\0";
	openFileDialog.lpstrFile = szFileName;
	openFileDialog.nMaxFile = MAX_PATH;
	openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	openFileDialog.lpstrDefExt = "";
	if (GetOpenFileName(&openFileDialog)){
		std::cout << "open : " << szFileName << "." << openFileDialog.lpstrDefExt << std::endl;
		m_app->OpenMap(szFileName);
	}
}

bool GUICallbacks::SaveAsMap1(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Window *saveWindow = myRoot->getChildRecursive("SaveMapWindow");

	CEGUI::Editbox *editBox = dynamic_cast<CEGUI::Editbox *>(saveWindow->getChildRecursive("MapNameEditbox"));

	m_app->getMapData()->setName(std::string(editBox->getText().c_str()));
	m_app->getMapData()->setFullPath(std::string("./../maps/" + std::string(editBox->getText().c_str()) + ".json"));
	m_app->SaveMap();
	
	editBox->setText("Map name");
	saveWindow->setVisible(false);

	return true;
}

void GUICallbacks::SaveAsMapThreadFunc()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Window *saveWindow = myRoot->getChildRecursive("SaveMapWindow");

	saveWindow->setVisible(true);

}

bool GUICallbacks::OpenMap(const CEGUI::EventArgs &)
{
	m_app->OpenMapWindow();

	return true;
}

bool GUICallbacks::ConfirmMapToLoad(const CEGUI::EventArgs &)
{
	m_app->ConfirmMapToLoad();

	return true;
}

bool GUICallbacks::OpenMapLoad(const CEGUI::EventArgs &)
{
	m_app->getMapData()->setModifications(false);
	m_app->OpenMap(m_app->mapToLoad);
	
	return true;
}

bool GUICallbacks::CancelLoad(const CEGUI::EventArgs &)
{
	m_app->mapToLoad = "";
	m_app->CancelOpen();

	return true;
}

bool GUICallbacks::SaveAsMap(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Window *saveWindow = myRoot->getChildRecursive("SaveMapWindow");

	saveWindow->setVisible(true);
	return true;
}

bool GUICallbacks::SaveAsMap()
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Window *saveWindow = myRoot->getChildRecursive("SaveMapWindow");

	saveWindow->setVisible(true);
	return true;
}

bool GUICallbacks::NewMap(const CEGUI::EventArgs &)
{
	m_app->NewMap();
	
	return true;
}

void GUICallbacks::setApp(GUIEngine *app)
{
	m_app = app;
}

bool GUICallbacks::SaveMap(const CEGUI::EventArgs &)
{
	m_app->SaveMap();
	return true;
}

static void checkEditedBox(CEGUI::Editbox *editedBox)
{
	if (editedBox->getText().length() == 0)
		editedBox->setText("0");
	CEGUI::String str = editedBox->getText();
	if (((str.at(0) >= '0' && str.at(0) <= '9') ||
		str.at(0) == '.' || str.at(0) == '-') == false)
	{
		editedBox->setText("0");
	}
}

bool GUICallbacks::XEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("X"));

		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();
		
		tmp->damage(
			std::stof(editedBox->getText().c_str()),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::YEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("Y"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			std::stof(editedBox->getText().c_str()),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}	
	return true;
}

bool GUICallbacks::XDirEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("dirX"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}	
	return true;
}

bool GUICallbacks::YDirEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("dirY"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}	
	return true;
}

bool GUICallbacks::XEndEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("endX"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			std::stof(editedBox->getText().c_str()),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}	
	return true;
}

bool GUICallbacks::YEndEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("endY"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			std::stof(editedBox->getText().c_str()),
			tmp->getWidth(),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::WidthEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("width"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			std::stof(editedBox->getText().c_str()),
			tmp->getHeight(),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::HeightEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("height"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			std::stof(editedBox->getText().c_str()),
			tmp->getTeam());

		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::PowerEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("power"));

		t_wallsNoDir *f = (t_wallsNoDir *)(item->_object);

		f->power = std::stof(editedBox->getText().c_str());
		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::SizeEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("size"));

		t_wallsNoDir *f = (t_wallsNoDir *)(item->_object);

		f->size = std::stof(editedBox->getText().c_str());
		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::TeamEditBoxChanged(const CEGUI::EventArgs &)
{
	CEGUI::Window *myRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	CEGUI::Listbox *box = static_cast<CEGUI::Listbox *>(myRoot->getChildRecursive("MapItems"));

	if (box->getSelectedCount() == 1)
	{
		CustomListBoxItem *item = (CustomListBoxItem *)box->getFirstSelectedItem();

		CEGUI::Editbox *editedBox = static_cast<CEGUI::Editbox *>(myRoot->getChildRecursive("team"));
		checkEditedBox(editedBox);

		AObject *tmp = S_Map->findObjectWithID(item->getID()).get();

		tmp->damage(
			tmp->getX(),
			tmp->getY(),
			tmp->getEndX(),
			tmp->getEndY(),
			tmp->getWidth(),
			tmp->getHeight(),
			std::stof(editedBox->getText().c_str()));

		m_app->getMapData()->setModifications(true);
	}
	return true;
}

bool GUICallbacks::HoverMenu(const CEGUI::EventArgs &)
{
	m_app->hoverMenu = true;
	Event::addEvent(ev_MOUSE_ENTERS_GUI);
	std::cout << "hover on" << std::endl;

	return true;
}

bool GUICallbacks::NotHoverMenu(const CEGUI::EventArgs &)
{
	m_app->hoverMenu = false;
	Event::addEvent(ev_MOUSE_LEAVES_GUI);

	return true;
}