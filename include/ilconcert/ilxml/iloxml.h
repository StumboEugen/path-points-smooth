// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/ilxml/iloxml.h
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
//
// 5725-A06 5725-A29 5724-Y47 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5655-Y21
// Copyright IBM Corp. 2000, 2013
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// ---------------------------------------------------------------------------
#ifndef __XML_iloxmlH
#define __XML_iloxmlH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/ilxml/ilolist.h>
#include <ilconcert/ilxml/iloiostream.h>

class IloXmlArray;

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlError
{
public:
	enum Severity {
		NoError,
		Warning,
		Error
	};
	static IloXmlBoolean	IsSet();
	static void		Set(Severity severity, int code, int line);
	static const char*	Get(Severity& severity, int& code, int& line);
	static void		Reset();
};

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlSpaces
{
public:
	IloXmlSpaces(int n_) : n(n_) {}
	int n;
};

IL_STDPREF ostream& operator<<(IL_STDPREF ostream&, const IloXmlSpaces&);

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlString
{
public:
	IloXmlString(const char* t_) : t(t_) {}
	const char* t;
};

ILO_EXPORTEDFUNCTION(IL_STDPREF ostream&)
operator<<(IL_STDPREF ostream&, const IloXmlString&);

// --------------------------------------------------------------------------
// Interface for XML nodes
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeI
{
public:
	virtual ~IloXmlNodeI();
	// ____________________________________________________________
	enum Type {
		Attribute,
		Element,
		Text,
		Comment,
		CData,
		PI,
		CharRef,
		EntityRef
	};
	virtual Type		getType() const = 0;
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream& stream) const = 0;
};

inline IL_STDPREF ostream&
operator<<(IL_STDPREF ostream& os, const IloXmlNodeI& node)
{
	return node.write(os);
}

// --------------------------------------------------------------------------
// Interface for XML node iterator
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeIteratorI
{
public:
	virtual ~IloXmlNodeIteratorI();
	virtual IloXmlBoolean	hasMoreNodes() const = 0;
	virtual IloXmlNodeI*	getNextNode() = 0;
};

// --------------------------------------------------------------------------
// Actual XML node iterator
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeIterator : public IloXmlNodeIteratorI
{
public:
	IloXmlNodeIterator(IloXmlNodeIteratorI* iterator)
		: _iterator(iterator) {}
	virtual ~IloXmlNodeIterator();
	// ____________________________________________________________
	virtual IloXmlBoolean	hasMoreNodes() const;
	virtual IloXmlNodeI*	getNextNode();

protected:
	IloXmlNodeIteratorI*	_iterator;
};

// --------------------------------------------------------------------------
// XML node list iterator
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeListIterator
	: public IloXmlNodeIteratorI,
	public IloXml_LIterator
{
public:
	IloXmlNodeListIterator(const IloXml_List& list);
	// ____________________________________________________________
	virtual IloXmlBoolean	hasMoreNodes() const;
	virtual IloXmlNodeI*	getNextNode();
};

// --------------------------------------------------------------------------
// XML node array iterator
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeArrayIterator : public IloXmlNodeIteratorI
{
public:
	IloXmlNodeArrayIterator(const IloXmlArray& objs);
	// ____________________________________________________________
	virtual IloXmlBoolean	hasMoreNodes() const;
	virtual IloXmlNodeI*	getNextNode();

protected:
	const IloXmlArray&	_nodeArray;
	IloXmlUInt		_currentIndex;
};

// --------------------------------------------------------------------------
// XML node pointer array iterator
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodePtrArrayIterator : public IloXmlNodeIteratorI
{
public:
	IloXmlNodePtrArrayIterator(IloXmlUInt count, const IloXmlNodeI** objs);
	// ____________________________________________________________
	virtual IloXmlBoolean	hasMoreNodes() const;
	virtual IloXmlNodeI*	getNextNode();

protected:
	IloXmlUInt		_count;
	const IloXmlNodeI**	_nodes;
	IloXmlUInt		_currentIndex;
};

// --------------------------------------------------------------------------
// Interface for attributes
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlAttributeI : public IloXmlNodeI
{
public:
	virtual ~IloXmlAttributeI();
	// ____________________________________________________________
	virtual const char*		getName() const = 0;
	virtual void		setName(const char* name);
	virtual void		storeName(char* name) = 0;
	virtual const char*		getValue() const = 0;
	virtual void		setValue(const char* value);
	virtual void		storeValue(char* value) = 0;
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream& stream) const;
	virtual Type		getType() const;

	static void			Write(IL_STDPREF ostream& stream,
		const char*         name,
		const char*         value);
	static void			Write(IL_STDPREF ostream& stream,
		const char*         name,
		IloXmlInt               value);
	static void			Write(IL_STDPREF ostream& stream,
		const char*         name,
		IloXmlDouble            value);
};

// --------------------------------------------------------------------------
// Default implementation of an attribute
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlAttribute : public IloXmlAttributeI
{
public:
	IloXmlAttribute(const char* name  = 0,
		const char* value = 0);
	virtual ~IloXmlAttribute();
	// ____________________________________________________________
	virtual const char*		getName() const;
	virtual void		storeName(char*);
	virtual const char*		getValue() const;
	virtual void		storeValue(char* name);

private:
	char*			_name;
	char*			_value;
};

// --------------------------------------------------------------------------
// Interface for elements.
// --------------------------------------------------------------------------
class IloXmlDocumentI;
class ILO_EXPORTED IloXmlElementI : public IloXmlNodeI
{
public:
	IloXmlElementI()
		: IloXmlNodeI()
	{}
	virtual ~IloXmlElementI();
	// ____________________________________________________________
	virtual Type		getType() const;
	virtual const char*		getTag() const = 0;
	virtual void		setTag(const char* tag);
	virtual void		storeTag(char* tag);
	virtual void		addChild(IloXmlNodeI* node);
	virtual void		addChild(IloXmlNodeI* node, IloXmlNodeI* nodeBefore);
	virtual void		storeAttribute(IloXmlDocumentI* document,
		char*           name,
		char*           value);
	virtual IloXmlElementI*	createChildElement(char* tag) const;
	virtual IloXmlAttributeI*	createAttribute(char* name,
		char* value) const;
	virtual void		addAttribute(IloXmlAttributeI* attribute);
	virtual IloXmlAttributeI*	getAttribute(const char* name) const;
	const char*			getAttributeValue(const char*) const;
	virtual IloXmlBoolean		doesPreserveWhiteSpaces() const;
	virtual void		preserveWhiteSpaces(IloXmlBoolean preserve);
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream& os) const;
	virtual void		writeAttributes(IL_STDPREF ostream& os) const;
	virtual IloXmlBoolean		writeChildren(IL_STDPREF ostream& os) const;
	virtual const char*		getText() const;
	virtual const char*		getCData() const;
	virtual const char*		getComment() const;
	static inline void		ResetIndent()    { _indent = 0; }
	static inline IloXmlUInt	GetIndentation() { return _indentation; }
	static inline void		SetIndentation(IloXmlUInt i) { _indentation = i; }
	static inline IloXmlUInt	GetIndent()      { return _indent; }
	static inline void		IndentMore()     { _indent += _indentation; }
	static inline void		IndentLess()     { _indent -= _indentation; }
	virtual IloXmlNodeIteratorI*	createChildrenIterator() const;
	virtual IloXmlNodeIteratorI*	createAttributesIterator() const;

private:
	static IloXmlUInt		_indent;
	static IloXmlUInt		_indentation;
};

//
// --------------------------------------------------------------------------
IloXmlDefineExpListOf(IloXmlAttributeI, IloXmlAttributeI*, ILO_EXPORTED);
IloXmlDefineExpListOf(IloXmlNodeI,      IloXmlNodeI*,      ILO_EXPORTED);
class ILO_EXPORTED IloXmlElement : public IloXmlElementI
{
public:
	IloXmlElement(const char* tag = 0);
	virtual ~IloXmlElement();
	// ____________________________________________________________
	virtual const char*		getTag() const;
	virtual void		storeTag(char*);

	IloXmlUInt			getChildrenCardinal() const;
	virtual void		addChild(IloXmlNodeI*);
	virtual void		addChild(IloXmlNodeI*, IloXmlNodeI*);
	inline IloXmlListOf(IloXmlNodeI)& getChildren() const
	{
		return ILOXML_CONSTCAST(IloXmlElement*,
			this)->_children;
	}

	IloXmlUInt			getAttributesCardinal() const;
	virtual void		addAttribute(IloXmlAttributeI*);
	inline IloXmlListOf(IloXmlAttributeI)& getAttributes() const
	{
		return ILOXML_CONSTCAST(IloXmlElement*,
			this)->_attributes;
	}
	virtual IloXmlAttributeI*	getAttribute(const char*) const;
	virtual IloXmlBoolean		doesPreserveWhiteSpaces() const;
	virtual void		preserveWhiteSpaces(IloXmlBoolean preserve);
	IloXmlElement*		getElement(const char* tagName,
		IloXmlNodeI* skip = 0) const;
	virtual const char*		getText() const;
	virtual const char*		getCData() const;
	virtual const char*		getComment() const;
	virtual IloXmlNodeIteratorI*	createChildrenIterator() const;
	virtual IloXmlNodeIteratorI* createAttributesIterator() const;

private:
	char*			_tag;
	IloXmlListOf(IloXmlNodeI)	_children;
	IloXmlListOf(IloXmlAttributeI)	_attributes;
	IloXmlBoolean			_preserveWS;
	IloXmlElement(const IloXmlElement&); //copy protection
};

// --------------------------------------------------------------------------
// Interface for Comment nodes.
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlCommentI : public IloXmlNodeI
{
public:
	virtual ~IloXmlCommentI();
	// ____________________________________________________________
	virtual const char*		getComment() const = 0;
	virtual void		setComment(const char*);
	virtual void		storeComment(char*) = 0;
	virtual Type		getType() const;
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream&) const;
};

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlComment : public IloXmlCommentI
{
public:
	IloXmlComment()
		: IloXmlCommentI(),
		_comment(0)
	{}
	virtual ~IloXmlComment();
	// ____________________________________________________________
	virtual const char*	getComment() const;
	virtual void	storeComment(char*);

private:
	char*		_comment;
};

// --------------------------------------------------------------------------
// Interface for Text nodes.
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlTextI : public IloXmlNodeI
{
public:
	virtual ~IloXmlTextI();
	// ____________________________________________________________
	virtual Type		getType() const;
	virtual const char*		getText() const = 0;
	virtual void		setText(const char*);
	virtual void		storeText(char*) = 0;
	virtual void		appendText(const char* text) = 0;
	virtual IL_STDPREF ostream& write(IL_STDPREF ostream&) const;
};

// --------------------------------------------------------------------------
// Default implementation of Text nodes.
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlText : public IloXmlTextI
{
public:
	IloXmlText()
		: IloXmlTextI(),
		_text(0)
	{}
	IloXmlText(char* text);
	virtual ~IloXmlText();
	// ____________________________________________________________
	virtual const char*	getText() const;
	virtual void	storeText(char*);
	virtual void	appendText(const char* text);

private:
	char*		_text;
};

// --------------------------------------------------------------------------
// class IloXmlPII
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlPII : public IloXmlNodeI
{
public:
	virtual ~IloXmlPII();
	// ____________________________________________________________
	virtual Type		getType() const;
	virtual const char*		getTarget() const = 0;
	virtual void		setTarget(const char*);
	virtual void		storeTarget(char*) = 0;
	virtual const char*		getData() const = 0;
	virtual void		setData(const char*);
	virtual void		storeData(char*) = 0;
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream&) const;
};

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlPI : public IloXmlPII
{
public:
	IloXmlPI()
		: IloXmlPII(),
		_target(0),
		_data(0)
	{
	}
	virtual ~IloXmlPI();
	// ____________________________________________________________
	virtual const char*	getTarget() const;
	virtual void	storeTarget(char*);
	virtual const char*	getData() const;
	virtual void	storeData(char*);

private:
	char*		_target;
	char*		_data;
};

// --------------------------------------------------------------------------
// class IloXmlCDataI
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlCDataI : public IloXmlNodeI
{
public:
	virtual ~IloXmlCDataI();
	// ____________________________________________________________
	virtual Type		getType() const;
	virtual const char*		getData() const = 0;
	virtual void		setData(const char*);
	virtual void		storeData(char*) = 0;
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream&) const;
};

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlCData : public IloXmlCDataI
{
public:
	IloXmlCData(char* text);

	IloXmlCData()
		: IloXmlCDataI(),
		_data(0)
	{
	}
	virtual ~IloXmlCData();
	// ____________________________________________________________
	virtual const char*	getData() const;
	virtual void	storeData(char*);

private:
	char*		_data;
};

// --------------------------------------------------------------------------
// class IloXmlCharRefI
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlCharRefI : public IloXmlNodeI
{
public:
	~IloXmlCharRefI();
	// ____________________________________________________________
	virtual Type		getType() const;
	virtual int			getRefCode() const = 0;
	virtual void		setRefCode(int code) = 0;
	virtual const char*		getRefName() const = 0;
	virtual void		setRefName(const char*);
	virtual void		storeRefName(char*) = 0;
	virtual IL_STDPREF ostream& write(IL_STDPREF ostream&) const;
};

// --------------------------------------------------------------------------
// class IloXmlCharRefElement
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlCharRef : public IloXmlCharRefI
{
public:
	IloXmlCharRef()
		: IloXmlCharRefI(),
		_refCode(0),
		_refName(0)
	{
	}
	virtual ~IloXmlCharRef();
	// ____________________________________________________________
	virtual int		getRefCode() const;
	virtual void	setRefCode(int code);
	virtual const char*	getRefName() const;
	virtual void	storeRefName(char*);

private:
	int			_refCode;
	char*		_refName;
};

// --------------------------------------------------------------------------
// Element factory for documents
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlNodeFactory
{
public:
	virtual ~IloXmlNodeFactory();
	// ____________________________________________________________
	IloXmlAttributeI*	createAttribute(IloXmlElementI* element,
		char*          name,
		char*          value) const;
	IloXmlElementI*	createElement(char* tag) const;
	IloXmlTextI*		createText() const;
	IloXmlCommentI*	createComment() const;
	IloXmlCharRefI*	createCharRef() const;
	IloXmlPII*		createPI() const;
	IloXmlCDataI*	createCData() const;
};

// --------------------------------------------------------------------------
// Interface for documents
// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlDocumentI
{
public:
	virtual ~IloXmlDocumentI();
	// ____________________________________________________________
	virtual void	addNode(IloXmlNodeI* node) = 0;
	virtual void	startElement(IloXmlElementI* element);
	virtual void	attributeAdded(IloXmlElementI*   node,
		IloXmlAttributeI* attribute);
	virtual void	endElement(IloXmlElementI* element);
	virtual void	textAdded(IloXmlTextI* text);
	virtual void	commentAdded(IloXmlCommentI* comment);
	virtual void	charRefAdded(IloXmlCharRefI* charRef);
	virtual void	processingInstructionAdded(IloXmlPII* pi);
	virtual void	cDataAdded(IloXmlCDataI* cdata);
	virtual IloXmlAttributeI*	createAttribute(IloXmlElementI* node,
		char*          name,
		char*          value) const;
	virtual IloXmlElementI*	createElement(char* tag) const;
	virtual IloXmlTextI*	createText() const;
	virtual IloXmlCommentI*	createComment() const;
	virtual IloXmlCharRefI*	createCharRef() const;
	virtual IloXmlPII*	createPI() const;
	virtual IloXmlCDataI*	createCData() const;
	virtual IloXmlNodeFactory*	getNodeFactory() const;
	virtual const char*	getVersion() const;
	virtual const char*	getDoctypeName() const;
	virtual const char*	getDoctypeRest() const;
	virtual void	empty();
	void		read(IL_STDPREF istream& stream);
	virtual IL_STDPREF ostream&	write(IL_STDPREF ostream& stream) const;
	virtual void	writeNodes(IL_STDPREF ostream& stream) const;
	virtual IloXmlNodeIteratorI*	createNodesIterator() const;

protected:
	virtual void	storeVersion(char* version);
	virtual void	storeDoctype(char* name, char* rest);

	friend class	IloXmlParser;
};

// --------------------------------------------------------------------------
class ILO_EXPORTED IloXmlDocument : public IloXmlDocumentI
{
public:
	IloXmlDocument();
	virtual ~IloXmlDocument();
	// ____________________________________________________________
	IloXmlListOf(IloXmlNodeI)& getNodes() const;
	IloXmlElement* getRootElement() const;
	virtual void		addNode(IloXmlNodeI*);
	virtual const char*		getVersion() const;
	virtual const char*		getDoctypeName() const;
	virtual const char*		getDoctypeRest() const;
	virtual void		empty();
	virtual IloXmlNodeIteratorI*	createNodesIterator() const;

protected:
	virtual void		storeVersion(char* version);
	virtual void		storeDoctype(char* name, char* rest);

private:
	IloXmlListOf(IloXmlNodeI)	_nodes;
	char*			_version;
	char*			_doctypeName;
	char*			_doctypeRest;
};
#ifdef _WIN32
#pragma pack(pop)
#endif
#endif 
