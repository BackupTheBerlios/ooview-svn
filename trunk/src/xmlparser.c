#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseStory (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"p"))) {			/* (keyword) */
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("name: %s\n", key);
			xmlFree(key);
			return;
		}
		cur = cur->next;
	}
	return;
}

static void parseDoc(char *docname) {
	
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
			
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
						
	cur = xmlDocGetRootElement(doc);
							
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	/* printf("%s\n", (char*) cur->name); */
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-content")) {				/* ROOT NODE (story) */
		fprintf(stderr,"document of the wrong type, root node != story\n");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))){			/* CHILD NODE (storyinfo) */
			cur = cur->xmlChildrenNode;
			while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *)"text")))
					parseStory (doc, cur);
				cur = cur->next;
			}
			
					
			//parseStory (doc, cur);
		}
															 
		if (cur != NULL) cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int main(int argc, char **argv) {

		char *docname;
				
		if (argc <= 1) {
			printf("Usage: %s docname\n", argv[0]);
			return(0);
		}
		docname = argv[1];
		parseDoc (docname);
			
		return (1);
}
