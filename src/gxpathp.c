#include <assert.h>
#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

int const exit_code_invalid_arg = 2;

void
execute_file(char const *filename, char const *pattern) {
    xmlDocPtr doc = xmlParseFile(filename);
    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    if (!ctxt) {
        xmlFreeDoc(doc);
        return;
    }

    xmlXPathObjectPtr xpath_obj = xmlXPathEval(BAD_CAST pattern, ctxt);
    if (!xpath_obj) {
        xmlXPathFreeContext(ctxt);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodeSetPtr const nodes = xpath_obj->nodesetval;
    int const size = nodes->nodeNr;
    for (int i = 0; i < size; ++i) {
        assert(nodes->nodeTab[i]);
        printf("%s:%s\n", filename, pattern);
    }

    xmlXPathFreeObject(xpath_obj);
    xmlXPathFreeContext(ctxt);
    xmlFreeDoc(doc);
}

int
main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s pattern file ...\n", argv[0]);
        return exit_code_invalid_arg;
    }

    for (int i = 2; i < argc; ++i) {
        execute_file(argv[i], argv[1]);
    }
    return 0;
}
