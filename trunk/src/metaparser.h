#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "main.h"

void parseText (xmlDocPtr doc, xmlNodePtr cur);
static void parseDoc (char *docname, struct fileinfo *buffer);
