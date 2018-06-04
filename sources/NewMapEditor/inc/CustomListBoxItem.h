#ifndef CUSTOMLISTBOXITEM_H_
#define CUSTOMLISTBOXITEM_H_

#include <CEGUI/CEGUI.h>
#include <string>
#include "MapStructures.h"

class CustomListBoxItem : public CEGUI::ListboxTextItem
{
public:
	CustomListBoxItem(const CEGUI::String &text, t_map_obj *, int item_id = 0, void *item_data = 0, bool disabled = false, bool auto_delete = true);
	~CustomListBoxItem();

	t_map_obj	*_object;
};

#endif