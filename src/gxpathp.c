#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

int const exit_code_invalid_arg = 2;

xmlXPathCompExprPtr
make_xpath_comp_expr(char const *pattern) {
    assert(pattern && (strlen(pattern) > 0));
    if (pattern[0] == '/') {
        return xmlXPathCompile(BAD_CAST pattern);
    }
    const char *prefix = "//";
    size_t len = strlen(prefix) + strlen(pattern);
    char *buf = (char *)(malloc(len + 1));
    strcpy(buf, prefix);
    strcat(buf, pattern);
    buf[len] = '\0';
    return xmlXPathCompile(BAD_CAST buf);
}

void
execute_file(char const *filename, xmlXPathCompExprPtr xpath) {
    xmlDocPtr doc = xmlParseFile(filename);
    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    if (!ctxt) {
        xmlFreeDoc(doc);
        return;
    }

    xmlXPathObjectPtr xpath_obj = xmlXPathCompiledEval(xpath, ctxt);
    if (!xpath_obj) {
        xmlXPathFreeContext(ctxt);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodeSetPtr const nodes = xpath_obj->nodesetval;
    int const size = nodes->nodeNr;
    for (int i = 0; i < size; ++i) {
        assert(nodes->nodeTab[i]);
        printf("%s:%s\n", filename, nodes->nodeTab[i]->name);
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

    if (!strlen(argv[1])) {
        printf("invalid pattern\n");
        return exit_code_invalid_arg;
    }
    xmlXPathCompExprPtr xpath = make_xpath_comp_expr(argv[1]);

    for (int i = 2; i < argc; ++i) {
        execute_file(argv[i], xpath);
    }
    xmlXPathFreeCompExpr(xpath);
    return 0;
}
