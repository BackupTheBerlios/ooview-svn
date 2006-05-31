#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "main.h"

void parseText (xmlDocPtr doc, xmlNodePtr cur);
void get_file_meta (char *docname, struct fileinfo *buffer);
