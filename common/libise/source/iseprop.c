#include "iseprop.h"

const char * whitespace_cb(mxml_node_t *node, int where)
{
	const char *name;

	name = mxmlGetElement(node);

	if (!strcmp(name, "html") ||
		!strcmp(name, "head") ||
		!strcmp(name, "body") ||
		!strcmp(name, "pre") ||
		!strcmp(name, "p") ||
		!strcmp(name, "h1") ||
		!strcmp(name, "h2") ||
		!strcmp(name, "h3") ||
		!strcmp(name, "h4") ||
		!strcmp(name, "h5") ||
		!strcmp(name, "h6"))
	{

		if (where == MXML_WS_BEFORE_OPEN ||
		where == MXML_WS_AFTER_CLOSE)
			return ("\n");
	}
	else if (!strcmp(name, "dl") ||
		!strcmp(name, "ol") ||
		!strcmp(name, "ul"))
	{
		return ("\n");
	}
	else if (!strcmp(name, "dd") ||
		!strcmp(name, "dt") ||
		!strcmp(name, "li"))
	{
		if (where == MXML_WS_BEFORE_OPEN)
			return ("\t");
		else if (where == MXML_WS_AFTER_CLOSE)
			return ("\n");
	}

	return (NULL);
}

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
	mxmlElementSetAttrf(items, "count", "%d", arr_cnt);

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
	}

	printf("prop xml result ::: %s\n", path);

	fp = fopen(path, "wb");
	mxmlSaveFile(xml, fp, whitespace_cb);
	fclose(fp);
}

prop_info_container parse_prop_xml(char *path)
{
	int arr_cnt,i;
	FILE *fp = fopen(path, "r");
	mxml_node_t	*xml, *prop, *key, *items, *item;
	secure_container **sc_array;
	prop_info_container prop_info;
	char **file_name;

	if (fp == NULL)
	{
		prop_info.status = ISE_STATUS_ERROR_IVALID_FILE;
		return prop_info;
	}

	xml = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	prop = mxmlFindElement(xml, xml, "prop", NULL, NULL, MXML_DESCEND);

	key = mxmlFindElement(prop, prop, "key", NULL, NULL, MXML_DESCEND);

	items = mxmlFindElement(prop, prop, "items", NULL, NULL, MXML_DESCEND);
	arr_cnt = atoi(mxmlElementGetAttr(items, "count"));
	//mxmlElementGetAttr(item, "name")
	sc_array = (secure_container**)malloc(sizeof(secure_container*) * arr_cnt);
	prop_info.file_name = (char**)malloc(sizeof(char*) * arr_cnt);

	for (item = mxmlFindElement(items, items, "item", NULL, NULL, MXML_DESCEND), i = 0; item != NULL; item = mxmlFindElement(item, items, "item", NULL, NULL, MXML_DESCEND), i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = atoi(mxmlElementGetAttr(item, "height"));
		sc_array[i]->width = atoi(mxmlElementGetAttr(item, "width"));
		sc_array[i]->pos_x = atoi(mxmlElementGetAttr(item, "x"));
		sc_array[i]->pos_y = atoi(mxmlElementGetAttr(item, "y"));
		
		
		prop_info.file_name[i] = mxmlElementGetAttr(item, "name");
	}

	prop_info.key_hash = mxmlElementGetAttr(key, "enc");
	prop_info.sc_arr = sc_array;
	prop_info.sc_count = arr_cnt;
	prop_info.status = ISE_STATUS_OK;

	fclose(fp);

	return prop_info;
}
