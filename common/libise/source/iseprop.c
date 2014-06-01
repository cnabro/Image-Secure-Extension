#include "iseprop.h"

int make_prop_xml(secure_container **sc_arr,int arr_cnt, char* path, int img_type)
{
	/*
		example of prop.xml
		<?xml version="1.0" encoding="UTF-8"
		<key enc="as89dfas89d7fa8sdf78"/>
		<items>
		<item path="item1.ise" x="300" y="400" width="300" height="300"/>
		<item path="item2.ise" x="300" y="400" width="300" height="300"/>
		<item path="item3.ise" x="300" y="400" width="300" height="300"/>
		</items>

		</xml>
		*/
	int	i;
	FILE* fp;
	int	fd;
	mxml_node_t	*xml, *prop, *key, *items, *item;
	mxml_index_t *ind;
	char buffer[16384];
	static const char *types[] = {
		"MXML_ELEMENT",
		"MXML_INTEGER",
		"MXML_OPAQUE",
		"MXML_REAL",
		"MXML_TEXT"
	};

	//tree = mxmlNewElement(MXML_NO_PARENT, "element");
	xml = mxmlNewXML("1.0");
	prop = mxmlNewElement(xml, "prop");

	key = mxmlNewElement(prop, "key");
	items = mxmlNewElement(prop, "items");

	mxmlElementSetAttr(key, "enc", "as89dfas89d7fa8sdf78");

	for (i = 0; i < arr_cnt; i++)
	{
		mxml_node_t *item = mxmlNewElement(items, "item");
		mxmlElementSetAttrf(item, "name", "item%d.ise",i);
		mxmlElementSetAttrf(item, "x", "%d", sc_arr[i]->pos_x);
		mxmlElementSetAttrf(item, "y", "%d", sc_arr[i]->pos_y);
		mxmlElementSetAttrf(item, "width", "%d", sc_arr[i]->width);
		mxmlElementSetAttrf(item, "height", "%d", sc_arr[i]->height);
		mxmlElementSetAttrf(item, "type", "%d", sc_arr[i]->type);
		/*switch (sc_arr[i]->type)
		{
			case ST_NORMAL:
				mxmlElementSetAttr(item, "type", "0");
				break;

			case ST_FACE:
				mxmlElementSetAttr(item, "type", "1");
				break;

			default:
				break;
		}*/
	}
	//mxmlNewInteger(mxmlNewElement(tree, "key"), 123);

	//mxmlAdd(tree);

	printf("prop xml result :::\n");
	if (mxmlSaveString(xml, buffer, sizeof(buffer), whitespace_cb) > 0)
	{

		printf("%s", buffer);
	}
		

}
