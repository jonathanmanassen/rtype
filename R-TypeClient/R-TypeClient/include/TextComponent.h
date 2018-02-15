#include "Component.h"

class TextComponent : public Component
{
	std::string	content;
	std::string	name;
	EntityPos	pos;
	bool		isEditable;
public:
	TextComponent(const std::string &_name, const bool &editStat) : name(_name), isEditable(editStat) {}
	const std::string	&getContent() const { return content; }
	const std::string	&getName() const { return name; }
	void	TextComponent::configure(const std::string &name, const float &x, const float &y);
	void				addLetter(char l) { content += l; }
	void				delLetter() { content = content.substr(0, content.size() - 1); }
	const bool			&getIsEditable() { return isEditable; }
	const EntityPos		&getPos() { return pos; }
};
