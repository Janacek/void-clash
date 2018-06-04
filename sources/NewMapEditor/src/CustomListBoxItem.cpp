#include "CustomListBoxItem.h"

CustomListBoxItem::CustomListBoxItem(const CEGUI::String &text, t_map_obj *object, int item_id, void *item_data, bool disabled, bool auto_delete)
: ListboxTextItem(text, item_id, item_data, disabled, auto_delete)
{
	_object = object;
}

CustomListBoxItem::~CustomListBoxItem()
{

}