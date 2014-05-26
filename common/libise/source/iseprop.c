#include "iseprop.h"

int make_prop_xml(secure_container **sc_arr, char* path, int img_type)
{
	int	i;
	FILE*fp;
	int	fd;
	mxml_node_t	*tree, *node;
	mxml_index_t *ind;
	char buffer[16384];
	static const char *types[] = {
		"MXML_ELEMENT",
		"MXML_INTEGER",
		"MXML_OPAQUE",
		"MXML_REAL",
		"MXML_TEXT"
	};

	tree = mxmlNewElement(MXML_NO_PARENT, "element");

	if (tree->type != MXML_ELEMENT)
	{
		mxmlDelete(tree);
		return -1;
	}

	mxmlNewInteger(tree, 123);
	if (mxmlSaveString(tree, buffer, sizeof(buffer), whitespace_cb) > 0)
	{
		printf("result : %s", buffer);
	}
		


	
}
