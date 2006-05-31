#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "metaparser.h"

void parseText (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	FILE *meta_inf;
	
	if ((meta_inf = fopen("/tmp/ooview/meta.inf","wt")) != NULL)
	{
			while (xmlStrcmp(cur->name,(const xmlChar *)"user-defined") )
			{
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"generator"))) {
			
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
					fprintf(meta_inf, "%s\n", key);
					xmlFree(key);			
 			    }
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"initial-creator"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 			    }
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"creation-date"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 		   		 }
				    if ((!xmlStrcmp(cur->name, (const xmlChar *)"creator"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
	 		    }
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"date"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 			    }
				    if ((!xmlStrcmp(cur->name, (const xmlChar *)"language"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 			    }
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"editing-cycles"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 			    }
			    if ((!xmlStrcmp(cur->name, (const xmlChar *)"editing-duration"))) {
		
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					fprintf(meta_inf, "%s\n", key);
				    xmlFree(key);			
 			    }

				cur = cur->next;
			}
			fclose(meta_inf);
	}
    return;
}

void get_file_meta(char *docname, struct fileinfo *buffer) {

	xmlDocPtr doc;
	xmlNodePtr cur;
	FILE *meta_inf;
	char *file_input = (char *)malloc(sizeof(char)*80);
	int i;
	
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		print_status_bar("Document not parsed successfully.");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		print_status_bar("empty document.");
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-meta")) {					/* root-element überprüfen */
		print_status_bar("Document of the wrong type, root node != document-meta");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;								/* eine ebene tiefer springen */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"meta"))){		/* so lange durchlaufen bis er den meta tag findet */
			cur = cur->xmlChildrenNode;								/* noch eine ebene tiefer */
			
			parseText (doc, cur);
			if ((meta_inf = fopen("/tmp/ooview/meta.inf","r")) != NULL)
			{
					for (i=0;i<8;i++)
					{
							fgets(file_input,80,meta_inf);
							switch (i)
							{
									case 0:
											buffer->generator = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->generator,file_input);
											break;
									case 1:
											buffer->initial_creator = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->initial_creator,file_input);
											break;
									case 2:
											buffer->creation_date = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->creation_date,file_input);
											break;
									case 3:
											buffer->creator = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->creator,file_input);
											break;
									case 4:
											buffer->date = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->date,file_input);
											break;
									case 5:
											buffer->language = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->language,file_input);
											break;
									case 6:
											buffer->editing_cycles = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->editing_cycles,file_input);
											break;
									case 7:
											buffer->editing_duration = (char *)malloc(sizeof(char)*80);
											strcpy(buffer->editing_duration,file_input);
											break;
							}
							print_status_bar(buffer->initial_creator);
							
					}
					
			}
			
		}
		if (cur != NULL)
				cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}
