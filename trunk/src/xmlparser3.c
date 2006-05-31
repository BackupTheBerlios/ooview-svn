/* 
 * odt-dokument enpacken;  * zb 
 * mkdir test; unzip -x test.odt -d test
 * $ ./xmlparser test/content.xml
 * kompilieren mit gcc -I/usr/include/libxml2 xmlparser3.c -o xmlparser3 -lxml2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseText (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *k1;
	xmlChar *k;
	xmlChar *k2;
	char *c1 = "";
	char *c2 = "";
	char *c3;
	cur = cur->xmlChildrenNode;
	xmlNodePtr temp_cur = cur;

	
	while (cur != NULL) {
			
	   	if ((!xmlStrcmp(cur->name, (const xmlChar *)"p"))) {
				
			temp_cur = cur->xmlChildrenNode;
			while (temp_cur != NULL) {
					
				if (!xmlStrcmp(temp_cur->name, (const xmlChar*)"span")) {
					k1 = xmlNodeListGetString(doc, temp_cur->xmlChildrenNode, 1);
					//c1 = (char*)k1;
					printf("%s", k1);
					xmlFree(k1);										
				}else {
					if (!xmlStrcmp(temp_cur->name, (const xmlChar*)"tab")) {
						printf("    ");
					} else {
						k2 = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
						// c2 = (char*)k2;
						printf("%s",k2);
	    	            xmlFree(k2);
					}
				}
				temp_cur = temp_cur->next;
			}
			printf("\n");
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
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-content")) {					/* root-element überprüfen */
		fprintf(stderr,"document of the wrong type, root node != document-content");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;								/* eine ebene tiefer springen */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))){		/* so lange durchlaufen bis er den body tag findet */
			cur = cur->xmlChildrenNode;								/* noch eine ebene tiefer */
			while (cur != NULL)	{
				if ((!xmlStrcmp(cur->name, (const xmlChar *)"text")))	/* so lange durchlaufen bis ein text tag gefunden */
						parseText (doc, cur);							
				cur = cur->next;
			}
		}
		if (cur != NULL) cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int
main(int argc, char **argv) {

	char *docname;
		
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);

	return (1);
}
