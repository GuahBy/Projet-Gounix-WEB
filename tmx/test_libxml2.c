/* Test simple pour vérifier que libxml2 fonctionne */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlversion.h>

int main() {
    printf("Test libxml2 pour Emscripten\n");
    printf("Version de libxml2: %s\n", LIBXML_DOTTED_VERSION);

    // Test simple de création d'un reader
    const char* xml_test = "<root><item>Test</item></root>";
    xmlTextReaderPtr reader = xmlReaderForMemory(xml_test, strlen(xml_test), NULL, NULL, 0);

    if (reader != NULL) {
        printf("✓ xmlReader créé avec succès!\n");
        printf("✓ libxml2 fonctionne correctement avec Emscripten!\n");
        xmlFreeTextReader(reader);
        return 0;
    } else {
        printf("✗ Erreur lors de la création du xmlReader\n");
        return 1;
    }
}
