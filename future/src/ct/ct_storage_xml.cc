/*
 * ct_storage_xml.cc
 *
 * Copyright 2017-2020 Giuseppe Penone <giuspen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "ct_storage_xml.h"
#include "ct_const.h"
#include "ct_misc_utils.h"
#include <libxml++/libxml++.h>
#include "ct_image.h"
#include "ct_codebox.h"
#include "ct_table.h"
#include "ct_main_win.h"


CtStorageXml::CtStorageXml(CtMainWin* pCtMainWin) : _pCtMainWin(pCtMainWin)
{

}

void CtStorageXml::close_connect()
{

}

void CtStorageXml::reopen_connect()
{

}

void CtStorageXml::test_connection()
{

}

bool CtStorageXml::populate_treestore(const Glib::ustring& file_path, Glib::ustring& error)
{
    try
    {
        // open file
        xmlpp::DomParser parser;
        parser.parse_file(file_path);
        if (!parser.get_document())
            throw std::runtime_error("document is null");
        if (parser.get_document()->get_root_node()->get_name() != CtConst::APP_NAME)
            throw std::runtime_error("document contains the wrong node root");

        // read bookmarks
        for (xmlpp::Node* xml_node :  parser.get_document()->get_root_node()->get_children("bookmarks"))
        {
            Glib::ustring bookmarks_csv = static_cast<xmlpp::Element*>(xml_node)->get_attribute_value("list");
            for (gint64& nodeId : CtStrUtil::gstring_split_to_int64(bookmarks_csv.c_str(), ","))
                _pCtMainWin->get_tree_store().bookmarks_add(nodeId);
        }

        // read nodes
        std::function<void(xmlpp::Element*, Gtk::TreeIter)> nodes_from_xml;
        nodes_from_xml = [&](xmlpp::Element* xml_element, Gtk::TreeIter parent_iter) {
            Gtk::TreeIter new_iter = _node_from_xml(xml_element, parent_iter);
            for (xmlpp::Node* xml_node : xml_element->get_children("node"))
                nodes_from_xml(static_cast<xmlpp::Element*>(xml_node), new_iter);
        };

        for (xmlpp::Node* xml_node: parser.get_document()->get_root_node()->get_children("node"))
            nodes_from_xml(static_cast<xmlpp::Element*>(xml_node), Gtk::TreeIter());

        return true;
    }
    catch (std::exception& e)
    {
        error = std::string("CtDocXmlStorage got exception: ") + e.what();
        return false;
    }
 }

bool CtStorageXml::save_treestore(const Glib::ustring& file_path, const CtStorageSyncPending&, Glib::ustring& error)
{
    try
    {
        xmlpp::Document xml_doc;
        xml_doc.create_root_node(CtConst::APP_NAME);

        // save bookmarks
        Glib::ustring rejoined;
        str::join_numbers(_pCtMainWin->get_tree_store().bookmarks_get(), rejoined, ",");
        xmlpp::Element* p_bookmarks_node = xml_doc.get_root_node()->add_child("bookmarks");
        p_bookmarks_node->set_attribute("list", rejoined);

        // save nodes
        auto ct_tree_iter = _pCtMainWin->get_tree_store().get_ct_iter_first();
        while (ct_tree_iter)
        {
            _nodes_to_xml(&ct_tree_iter, xml_doc.get_root_node());
            ct_tree_iter++;
        }

        // write file
        xml_doc.write_to_file(file_path);

        return true;
    }
    catch (std::exception& e)
    {
        error = e.what();
        return false;
    }
}

Glib::RefPtr<Gsv::Buffer> CtStorageXml::get_delayed_text_buffer(const gint64&,
                                                                   const std::string&,
                                                                   std::list<CtAnchoredWidget*>&) const
{
    return Glib::RefPtr<Gsv::Buffer>();
}

Gtk::TreeIter CtStorageXml::_node_from_xml(xmlpp::Element* xml_element, Gtk::TreeIter parent_iter)
{
    CtNodeData node_data;
    node_data.nodeId = CtStrUtil::gint64_from_gstring(xml_element->get_attribute_value("unique_id").c_str());
    node_data.name = xml_element->get_attribute_value("name");
    node_data.syntax = xml_element->get_attribute_value("prog_lang");
    node_data.tags = xml_element->get_attribute_value("tags");
    node_data.isRO = CtStrUtil::is_str_true(xml_element->get_attribute_value("readonly"));
    node_data.customIconId = (guint32)CtStrUtil::gint64_from_gstring(xml_element->get_attribute_value("custom_icon_id").c_str());
    node_data.isBold = CtStrUtil::is_str_true(xml_element->get_attribute_value("is_bold"));
    node_data.foregroundRgb24 = xml_element->get_attribute_value("foreground");
    node_data.tsCreation = CtStrUtil::gint64_from_gstring(xml_element->get_attribute_value("ts_creation").c_str());
    node_data.tsLastSave = CtStrUtil::gint64_from_gstring(xml_element->get_attribute_value("ts_lastSave").c_str());
    node_data.rTextBuffer = CtStorageXmlHelper(_pCtMainWin).create_buffer_and_widgets_from_xml(xml_element, node_data.syntax, node_data.anchoredWidgets, nullptr, -1);

    return _pCtMainWin->get_tree_store().append_node(&node_data, &parent_iter);
}

void CtStorageXml::_nodes_to_xml(CtTreeIter* ct_tree_iter, xmlpp::Element* p_node_parent)
{
    xmlpp::Element* p_node_node =  CtStorageXmlHelper(_pCtMainWin).node_to_xml(ct_tree_iter, p_node_parent, true);
    CtTreeIter ct_tree_iter_child = ct_tree_iter->first_child();
    while (ct_tree_iter_child)
    {
        _nodes_to_xml(&ct_tree_iter_child, p_node_node);
        ct_tree_iter_child++;
    }
}




CtStorageXmlHelper::CtStorageXmlHelper(CtMainWin* pCtMainWin) : _pCtMainWin(pCtMainWin)
{

}

xmlpp::Element* CtStorageXmlHelper::node_to_xml(CtTreeIter* ct_tree_iter, xmlpp::Element* p_node_parent, bool with_widgets)
{
    xmlpp::Element* p_node_node = p_node_parent->add_child("node");
    p_node_node->set_attribute("name", ct_tree_iter->get_node_name());
    p_node_node->set_attribute("unique_id", std::to_string(ct_tree_iter->get_node_id()));
    p_node_node->set_attribute("prog_lang", ct_tree_iter->get_node_syntax_highlighting());
    p_node_node->set_attribute("tags", ct_tree_iter->get_node_tags());
    p_node_node->set_attribute("readonly", std::to_string(ct_tree_iter->get_node_read_only()));
    p_node_node->set_attribute("custom_icon_id", std::to_string(ct_tree_iter->get_node_custom_icon_id()));
    p_node_node->set_attribute("is_bold", std::to_string(ct_tree_iter->get_node_is_bold()));
    p_node_node->set_attribute("foreground", ct_tree_iter->get_node_foreground());
    p_node_node->set_attribute("ts_creation", std::to_string(ct_tree_iter->get_node_creating_time()));
    p_node_node->set_attribute("ts_lastsave", std::to_string(ct_tree_iter->get_node_modification_time()));

    Glib::RefPtr<Gsv::Buffer> buffer = ct_tree_iter->get_node_text_buffer();
    save_buffer_no_widgets_to_xml(p_node_node, buffer, 0, -1, 'n');

    if (with_widgets)
        for (CtAnchoredWidget* pAnchoredWidget : ct_tree_iter->get_embedded_pixbufs_tables_codeboxes())
            pAnchoredWidget->to_xml(p_node_node, 0);

    return p_node_node;
}

Glib::RefPtr<Gsv::Buffer> CtStorageXmlHelper::create_buffer_and_widgets_from_xml(xmlpp::Element* parent_xml_element, const Glib::ustring& syntax,
                                                                    std::list<CtAnchoredWidget*>& widgets, Gtk::TextIter* text_insert_pos, int force_offset)
{
    Glib::RefPtr<Gsv::Buffer> buffer = _pCtMainWin->get_new_text_buffer(syntax);
    buffer->begin_not_undoable_action();
    for (xmlpp::Node* xml_slot : parent_xml_element->get_children())
        get_text_buffer_one_slot_from_xml(buffer, xml_slot, widgets, text_insert_pos, force_offset);
    buffer->end_not_undoable_action();
    buffer->set_modified(false);
    return buffer;
}

void CtStorageXmlHelper::get_text_buffer_one_slot_from_xml(Glib::RefPtr<Gsv::Buffer> buffer, xmlpp::Node* slot_node,
                                                 std::list<CtAnchoredWidget*>& widgets, Gtk::TextIter* text_insert_pos, int force_offset)
{
    xmlpp::Element* slot_element = static_cast<xmlpp::Element*>(slot_node);
    Glib::ustring slot_element_name = slot_element->get_name();
    if (slot_element_name == "rich_text")
    {
        _add_rich_text_from_xml(buffer, slot_element, text_insert_pos);
    }
    else if (slot_element_name != "node")
    {
        const int char_offset = force_offset != -1 ? force_offset : std::stoi(slot_element->get_attribute_value("char_offset"));
        Glib::ustring justification = slot_element->get_attribute_value(CtConst::TAG_JUSTIFICATION);
        if (justification.empty()) justification = CtConst::TAG_PROP_VAL_LEFT;

        CtAnchoredWidget* widget{nullptr};
        if (slot_element_name == "encoded_png")  widget = _create_image_from_xml(slot_element, char_offset, justification);
        else if (slot_element_name == "table")   widget = _create_table_from_xml(slot_element, char_offset, justification);
        else if (slot_element_name == "codebox") widget = _create_codebox_from_xml(slot_element, char_offset, justification);
        if (widget)
        {
            widget->insertInTextBuffer(buffer);
            widgets.push_back(widget);
        }
    }

}


Glib::RefPtr<Gsv::Buffer> CtStorageXmlHelper::create_buffer_no_widgets(const Glib::ustring& syntax, const char* xml_content)
{
    xmlpp::DomParser parser;
    parser.parse_memory(xml_content);
    std::list<CtAnchoredWidget*> widgets;
    if (parser.get_document() && parser.get_document()->get_root_node())
        return create_buffer_and_widgets_from_xml(parser.get_document()->get_root_node(), syntax, widgets, nullptr, -1);
    return Glib::RefPtr<Gsv::Buffer>();
}

bool CtStorageXmlHelper::populate_table_matrix(std::vector<std::vector<CtTableCell*>>& tableMatrix, const char* xml_content)
{
    xmlpp::DomParser parser;
    parser.parse_memory(xml_content);
    if (parser.get_document() && parser.get_document()->get_root_node())
    {
        populate_table_matrix(tableMatrix, parser.get_document()->get_root_node());
        return true;
    }
    return false;
}

void CtStorageXmlHelper::populate_table_matrix(std::vector<std::vector<CtTableCell*>>& tableMatrix, xmlpp::Element* xml_element)
{
    for (xmlpp::Node* pNodeRow : xml_element->get_children("row"))
    {
        tableMatrix.push_back(CtTableRow{});
        for (xmlpp::Node* pNodeCell : pNodeRow->get_children("cell"))
        {
            xmlpp::TextNode* pTextNode = static_cast<xmlpp::Element*>(pNodeCell)->get_child_text();
            const Glib::ustring textContent = pTextNode ? pTextNode->get_content() : "";
            tableMatrix.back().push_back(new CtTableCell(_pCtMainWin, textContent, CtConst::TABLE_CELL_TEXT_ID));
        }
    }
    bool head_back = xml_element->get_attribute_value("head_front").empty();
    if (head_back)
    {
        tableMatrix.insert(tableMatrix.begin(), tableMatrix.back());
        tableMatrix.pop_back();
    }
}

/*static*/ void CtStorageXmlHelper::save_buffer_no_widgets_to_xml(xmlpp::Element* p_node_parent, Glib::RefPtr<Gtk::TextBuffer> buffer,
                                                       int start_offset, int end_offset, const gchar change_case)
{
    auto rich_txt_serialize = [&](Gtk::TextIter& start_iter, Gtk::TextIter& end_iter, std::map<const gchar*, std::string>& curr_attributes) {
         xmlpp::Element* p_rich_text_node = p_node_parent->add_child("rich_text");
         for (const auto& map_iter : curr_attributes)
         {
             if (!map_iter.second.empty())
                p_rich_text_node->set_attribute(map_iter.first, map_iter.second);
         }
         Glib::ustring slot_text = start_iter.get_text(end_iter);
         if ('n' != change_case)
         {
             if ('l' == change_case) slot_text = slot_text.lowercase();
             else if ('u' == change_case) slot_text = slot_text.uppercase();
             else if ('t' == change_case) slot_text = str::swapcase(slot_text);
         }
         p_rich_text_node->add_child_text(slot_text);
    };

    CtTextIterUtil::generic_process_slot(start_offset, end_offset, buffer, rich_txt_serialize);
}

void CtStorageXmlHelper::_add_rich_text_from_xml(Glib::RefPtr<Gsv::Buffer> buffer, xmlpp::Element* xml_element, Gtk::TextIter* text_insert_pos)
{
    xmlpp::TextNode* text_node = xml_element->get_child_text();
    if (!text_node) return;
    const Glib::ustring text_content = text_node->get_content();
    if (text_content.empty()) return;
    std::vector<Glib::ustring> tags;
    for (const xmlpp::Attribute* pAttribute : xml_element->get_attributes())
    {
        if (CtStrUtil::is_pgchar_in_pgchar_iterable(pAttribute->get_name().c_str(), CtConst::TAG_PROPERTIES))
            tags.push_back(_pCtMainWin->get_text_tag_name_exist_or_create(pAttribute->get_name(), pAttribute->get_value()));
    }
    Gtk::TextIter iter = text_insert_pos ? *text_insert_pos : buffer->end();
    if (tags.size() > 0)
        buffer->insert_with_tags_by_name(iter, text_content, tags);
    else
        buffer->insert(iter, text_content);
}

CtAnchoredWidget* CtStorageXmlHelper::_create_image_from_xml(xmlpp::Element* xml_element, int charOffset, const Glib::ustring& justification)
{
    const Glib::ustring anchorName = xml_element->get_attribute_value("anchor");
    if (!anchorName.empty())
        return new CtImageAnchor(_pCtMainWin, anchorName, charOffset, justification);

    const Glib::ustring file_name = xml_element->get_attribute_value("filename");
    xmlpp::TextNode* pTextNode = xml_element->get_child_text();
    const std::string encodedBlob = pTextNode ? pTextNode->get_content() : "";
    const std::string rawBlob = Glib::Base64::decode(encodedBlob);
    if (not file_name.empty())
    {
        std::string timeStr = xml_element->get_attribute_value("time");
        if (timeStr.empty())
        {
            timeStr = "0";
        }
        double timeDouble = std::stod(timeStr);
        return new CtImageEmbFile(_pCtMainWin, file_name, rawBlob, timeDouble, charOffset, justification);
    }
    else
    {
        const Glib::ustring link = xml_element->get_attribute_value("link");
        return new CtImagePng(_pCtMainWin, rawBlob, link, charOffset, justification);
    }

}

CtAnchoredWidget* CtStorageXmlHelper::_create_codebox_from_xml(xmlpp::Element* xml_element, int charOffset, const Glib::ustring& justification)
{
    xmlpp::TextNode* pTextNode = xml_element->get_child_text();
    const Glib::ustring textContent = pTextNode ? pTextNode->get_content() : "";
    const Glib::ustring syntaxHighlighting = xml_element->get_attribute_value("syntax_highlighting");
    const int frameWidth = std::stoi(xml_element->get_attribute_value("frame_width"));
    const int frameHeight = std::stoi(xml_element->get_attribute_value("frame_height"));
    const bool widthInPixels = CtStrUtil::is_str_true(xml_element->get_attribute_value("width_in_pixels"));
    const bool highlightBrackets = CtStrUtil::is_str_true(xml_element->get_attribute_value("highlight_brackets"));
    const bool showLineNumbers = CtStrUtil::is_str_true(xml_element->get_attribute_value("show_line_numbers"));

    return new CtCodebox(_pCtMainWin,
                        textContent,
                        syntaxHighlighting,
                        frameWidth,
                        frameHeight,
                        charOffset,
                        justification,
                        widthInPixels,
                        highlightBrackets,
                        showLineNumbers);
}

CtAnchoredWidget* CtStorageXmlHelper::_create_table_from_xml(xmlpp::Element* xml_element, int charOffset, const Glib::ustring& justification)
{
    const int colMin = std::stoi(xml_element->get_attribute_value("col_min"));
    const int colMax = std::stoi(xml_element->get_attribute_value("col_max"));

    CtTableMatrix tableMatrix;
    populate_table_matrix(tableMatrix, xml_element);

    return new CtTable(_pCtMainWin, tableMatrix, colMin, colMax, charOffset, justification);
}


