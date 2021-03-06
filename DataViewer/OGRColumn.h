/**
 * GeoDa TM, Copyright (C) 2011-2014 by Luc Anselin - all rights reserved
 *
 * This file is part of GeoDa.
 * 
 * GeoDa is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GeoDa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GEODA_CENTER_OGR_COLUMN_H__
#define __GEODA_CENTER_OGR_COLUMN_H__

#include <vector>
#include <map>

#include "../GdaConst.h"
#include "../DataViewer/VarOrderPtree.h"
#include "../DataViewer/VarOrderMapper.h"
#include "../ShapeOperations/OGRLayerProxy.h"

using namespace std;

/**
 *
 */
class OGRColumn
{
protected:
    wxString name;
    int  length;
    int  decimals;
    bool is_new;
    bool is_deleted;
    int  rows;
    OGRLayerProxy* ogr_layer;
    // markers for a new column if the cell has ben assigned a value
    vector<bool> set_markers;
public:
    OGRColumn(OGRLayerProxy* _ogr_layer,
              wxString name, int field_length, int decimals);
    OGRColumn(OGRLayerProxy* _ogr_layer, int idx);
    virtual ~OGRColumn(){}
    
    int GetColIndex();
    void UpdateOGRLayer(OGRLayerProxy* new_ogr_layer);
    bool IsNewColumn() { return is_new;}
    bool IsCellUpdated(int row);
    bool IsDeleted() { return is_deleted;}
    void SetDeletion(bool is_delete) { is_deleted = is_delete;}
    OGRLayerProxy* GetOGRLayerProxy() { return ogr_layer;}
    wxString GetName();
    void Rename(const wxString& new_name);
    int GetLength();
    void SetLength(int new_length);
    int GetDecimals();
    void SetDecimals(int new_decimals);
    int GetNumRows() { return rows; }
    // only for adding new column, and rollback() operation
    //void SetColIndex(int idx) { col_idx = idx;}
    // virtual functions that need to be overwritten
    virtual bool IsUndefined(int row);
    virtual GdaConst::FieldType GetType() {return GdaConst::unknown_type;}
    virtual void UpdateData(const vector<double>& data);
    virtual void UpdateData(const vector<wxInt64>& data);
    virtual void UpdateData(const vector<wxString>& data);
	virtual void GetCellValue(int row, wxInt64& val);
	virtual void GetCellValue(int row, double& val);
	virtual void GetCellValue(int row, wxString& val);
    // interfaces for TableInterface
    virtual void FillData(vector<double>& data) = 0;
    virtual void FillData(vector<wxInt64>& data) = 0;
    virtual void FillData(vector<wxString>& data) = 0;
    virtual wxString GetValueAt(int row_idx, int disp_decimals=0,
                                wxCSConv* m_wx_encoding=NULL) = 0;
    virtual void SetValueAt(int row_idx, const wxString& value) = 0;
};

/**
 *
 */
class OGRColumnInteger : public OGRColumn
{
private:
    vector<wxInt64> new_data;
    void InitMemoryData();
    
public:
    OGRColumnInteger(OGRLayerProxy* ogr_layer,
                     wxString name, int field_length, int decimals);
    OGRColumnInteger(OGRLayerProxy* ogr_layer, int idx);
    ~OGRColumnInteger();
    
    virtual GdaConst::FieldType GetType() {return GdaConst::long64_type;}
    virtual void FillData(vector<double>& data);
    virtual void FillData(vector<wxInt64>& data);
    virtual void FillData(vector<wxString>& data);
    virtual void UpdateData(const vector<wxInt64>& data);
    virtual void UpdateData(const vector<double>& data);
	virtual void GetCellValue(int row, wxInt64& val);
    virtual wxString GetValueAt(int row_idx, int disp_decimals=0,
                                wxCSConv* m_wx_encoding=NULL);
    virtual void SetValueAt(int row_idx, const wxString& value);
};

/**
 *
 */
class OGRColumnDouble : public OGRColumn
{
private:
    vector<double> new_data;
    void InitMemoryData();
    
public:
    OGRColumnDouble(OGRLayerProxy* ogr_layer,
                    wxString name, int field_length, int decimals);
    OGRColumnDouble(OGRLayerProxy* ogr_layer, int idx);
    ~OGRColumnDouble();
    
    virtual GdaConst::FieldType GetType() {return GdaConst::double_type;}
    virtual void FillData(vector<double>& data);
    virtual void FillData(vector<wxInt64>& data);
    virtual void FillData(vector<wxString>& data);
    virtual void UpdateData(const vector<wxInt64>& data);
    virtual void UpdateData(const vector<double>& data);
	virtual void GetCellValue(int row, double& val);
    virtual wxString GetValueAt(int row_idx, int disp_decimals=0,
                                wxCSConv* m_wx_encoding=NULL);
    virtual void SetValueAt(int row_idx, const wxString& value);
};

/**
 *
 */
class OGRColumnString : public OGRColumn
{
private:
    vector<wxString> new_data;
    void InitMemoryData();
    
public:
    OGRColumnString(OGRLayerProxy* ogr_layer,
                    wxString name, int field_length, int decimals);
    OGRColumnString(OGRLayerProxy* ogr_layer, int idx);
    ~OGRColumnString();
    
    virtual GdaConst::FieldType GetType() {return GdaConst::string_type;}
    virtual void FillData(vector<double>& data);
    virtual void FillData(vector<wxInt64>& data);
    virtual void FillData(vector<wxString>& data);
    virtual void UpdateData(const vector<wxString>& data);
    virtual void UpdateData(const vector<wxInt64>& data);
    virtual void UpdateData(const vector<double>& data);
	virtual void GetCellValue(int row, wxString& val);
    virtual wxString GetValueAt(int row_idx, int disp_decimals=0,
                                wxCSConv* m_wx_encoding=NULL);
    virtual void SetValueAt(int row_idx, const wxString& value);
};

/**
 *
 */
class OGRColumnDate: public OGRColumn
{
private:
    vector<wxInt64> new_data;
    void InitMemoryData();
    
public:
    // XXX: don't support add new date column yet
    //OGRColumnDate(int rows);
    OGRColumnDate(OGRLayerProxy* ogr_layer, int idx);
    ~OGRColumnDate();
    
    virtual void FillData(vector<double>& data);
    virtual void FillData(vector<wxInt64>& data);
    virtual void FillData(vector<wxString>& data);
    
    virtual GdaConst::FieldType GetType() {return GdaConst::date_type;}
	virtual void GetCellValue(int row, wxInt64& val);
    virtual wxString GetValueAt(int row_idx, int disp_decimals=0,
                                wxCSConv* m_wx_encoding=NULL);
    virtual void SetValueAt(int row_idx, const wxString& value);
};

#endif
