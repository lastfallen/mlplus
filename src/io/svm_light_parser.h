#pragma warning( disable : 4786 )


#ifndef __SVMLIGHT_PARSER_H
#define __SVMLIGHT_PARSER_H

#include <fstream>
#include <sstream>
#include "GenericParser.h"
#include "NameMap.h"
#include "InputData.h"
#include "Utils/ClassHierarchy.h"

using namespace std;

namespace MultiBoost
{

class SVMLightParser : public GenericParser
{
public:
    SVMLightParser(const string& fileName, const string& headerFileName);

    virtual void readData(vector<Example>& examples, NameMap& classMap,
                          vector<NameMap>& enumMaps, NameMap& attributeNameMap,
                          vector<RawData::eAttributeType>& attributeTypes);

    virtual int  getNumAttributes() const
    {
        return _numAttributes;
    }

protected:
    /**
     Read the header in the following form:
     First line: 1 2 4 6 # class labels
     Second line: id1 id2 id3 # feature names
     Third line: 0.2 0.2 0.1 0.5 # label weights (optional )
     */
    void readHeader(NameMap& classMap,
                    vector<NameMap>& enumMaps, NameMap& attributeNameMap,
                    vector<RawData::eAttributeType>& attributeTypes);

    void readData(ifstream& in, vector<Example>& examples, NameMap& classMap,
                  vector<NameMap>& enumMaps, NameMap& attributeNameMap,
                  vector<RawData::eAttributeType>& attributeTypes);

    /**
     * If no header file is given, it is assumed that the labesl are 0..(K-1) and the features
     * are denoted by 1..F. This function finds the label and feauter having highest index.
     * \data 27/07/2011
     */
    void getHeaderInfoFromData(ifstream& in, NameMap& classMap,
                               vector<NameMap>& enumMaps, NameMap& attributeNameMap,
                               vector<RawData::eAttributeType>& attributeTypes);

    string readName(ifstream& in);


    void readDenseValues(ifstream& in, vector<FeatureReal>& values,
                         vector<NameMap>& enumMaps,
                         const vector<RawData::eAttributeType>& attributeTypes);

    void readSparseValues(istringstream& ss, vector<FeatureReal>& values, vector<int>& idxs, map<int, int>& idxmap,
                          vector<NameMap>& enumMaps,
                          const vector<RawData::eAttributeType>& attributeTypes, NameMap& attributeNameMap);

    void allocateSimpleLabels(vector<int>& labelIdx, vector<Label>& labels,
                              NameMap& classMap);

    int					 _numAttributes;

    locale				 _denseLocale;
    locale				 _sparseLocale;
    locale				 _labelLocale;
    bool				 _hasName;
    map< int, AlphaReal> _weightOfClasses; //! In the header file it is possible to define initial weights for classes

    size_t				 _numRows;
public:

};

// -----------------------------------------------------------------------------

inline string SVMLightParser::readName(ifstream& in)
{
    const locale& originalLocale = in.imbue(_denseLocale);
    string name;
    in >> name;
    in.imbue(originalLocale);
    return name;
}

// -----------------------------------------------------------------------------


} // end of namespace MultiBoost

#endif // __ARFF_PARSER_H
