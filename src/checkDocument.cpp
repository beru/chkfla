
#include "checkDocument.h"

static
void iterate(
	const std::string& name,
	const pugi::xml_node& target,
	const pugi::xml_node& check
	)
{
	if (strcmp(target.name(), check.name()) != 0) {
		printf("node name different\n");
		return;
	}
	
	for (pugi::xml_node node = check.first_child(); node; node=node.next_sibling()) {
		const char* nodeName = node.name();
		bool hasChild = node.first_child();
		bool bFound = false;
		for (pugi::xml_node tnode=target.first_child(); tnode; tnode=tnode.next_sibling()) {
			if (strcmp(tnode.name(), nodeName) == 0) {
				bFound = true;
				if (hasChild) {
					if (tnode.first_child()) {
						iterate(
							name + "." + tnode.name(),
							tnode,
							node
							);
					}else {
						printf("child node not found\n");
					}
				}else {
					if (strcmp(node.value(), tnode.value()) != 0) {
						printf("%s %s %s \n", name.c_str(), node.value(), tnode.value());
					}
				}
				break;
			}
		}
		if (!bFound) {
			printf("%s not found\n", nodeName);
		}
	}
}

void checkDocument(const pugi::xml_document& target, const pugi::xml_document& check)
{
	iterate(
		check.first_child().name(),
		target.first_child(),
		check.first_child()
		);
}

