#include "PHG4Parameters.h"

#include <pdbcalbase/PdbBankManager.h>
#include <pdbcalbase/PdbApplication.h>
#include <pdbcalbase/PdbBankList.h>
#include <pdbcalbase/PdbCalBank.h>
#include <pdbcalbase/PdbParameterMap.h>

#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHTimeStamp.h>

#include <TFile.h>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

// this is an ugly hack, the gcc optimizer has a bug which
// triggers the uninitialized variable warning which
// stops compilation because of our -Werror 
#include <boost/version.hpp> // to get BOOST_VERSION
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 4 && BOOST_VERSION == 105700 )
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma message "ignoring bogus gcc warning in boost header lexical_cast.hpp"
#include <boost/lexical_cast.hpp>
#pragma GCC diagnostic warning "-Wuninitialized"
#else
#include <boost/lexical_cast.hpp>
#endif


#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

void
PHG4Parameters::set_int_param(const std::string &name, const int ival)
{
  intparams[name] = ival;
}

int
PHG4Parameters::get_int_param(const std::string &name) const
{
  if (intparams.find(name) != intparams.end())
    {
      return intparams.find(name)->second;
    }
  cout << PHWHERE << " integer parameter " << name << " does not exist (forgot to set?)" << endl;

  exit(1);
}

void
PHG4Parameters::printint() const
{
  cout << "int parameters: " << endl;
  for (map<const string, int>::const_iterator iter = intparams.begin(); iter != intparams.end(); ++iter)
    {
      cout << iter->first << ": " << iter->second << endl;
    }
  return;
}

void
PHG4Parameters::set_double_param(const std::string &name, const double dval)
{
  doubleparams[name] = dval;
}

double
PHG4Parameters::get_double_param(const std::string &name) const
{
  if (doubleparams.find(name) != doubleparams.end())
    {
      return doubleparams.find(name)->second;
    }
  cout << PHWHERE << " double parameter " << name << " does not exist (forgot to set?)" << endl;

  exit(1);
}

void
PHG4Parameters::print() const
{
  cout << "Parameters for " << detname << endl;
  printint();
  printdouble();
  printstring();
  return;
}

void
PHG4Parameters::printdouble() const
{
  cout << "double parameters: " << endl;
  for (map<const string, double>::const_iterator iter = doubleparams.begin(); iter != doubleparams.end(); ++iter)
    {
      cout << iter->first << ": " << iter->second << endl;
    }
  return;
}

void
PHG4Parameters::set_string_param(const std::string &name, const string &str)
{
  stringparams[name] = str;
}

string
PHG4Parameters::get_string_param(const std::string &name) const
{
  if (stringparams.find(name) != stringparams.end())
    {
      return stringparams.find(name)->second;
    }
  cout << PHWHERE << " string parameter " << name << " does not exist (forgot to set?)" << endl;

  exit(1);
}

void
PHG4Parameters::printstring() const
{
  cout << "string parameters: " << endl;
  for (map<const string, string>::const_iterator iter = stringparams.begin(); iter != stringparams.end(); ++iter)
    {
      cout << iter->first << ": " << iter->second << endl;
    }
  return;
}


void
PHG4Parameters::FillFrom(const PdbParameterMap *saveparams)
{
  pair<std::map<const std::string, double>::const_iterator, std::map<const std::string, double>::const_iterator> begin_end_d = saveparams->get_dparam_iters();
  for (map<const std::string, double>::const_iterator iter = begin_end_d.first; iter != begin_end_d.second;++iter)
    {
      doubleparams[iter->first] = iter->second;
    }
  pair<std::map<const std::string, int>::const_iterator, std::map<const std::string, int>::const_iterator> begin_end_i = saveparams->get_iparam_iters();
  for (map<const std::string, int>::const_iterator iter = begin_end_i.first; iter != begin_end_i.second;++iter)
    {
      intparams[iter->first] = iter->second;
    }
  pair<std::map<const std::string, string>::const_iterator, std::map<const std::string, string>::const_iterator> begin_end_s = saveparams->get_cparam_iters();
  for (map<const std::string, string>::const_iterator iter = begin_end_s.first; iter != begin_end_s.second;++iter)
    {
      stringparams[iter->first] = iter->second;
    }

  return;
}


void
PHG4Parameters::SaveToNodeTree(PHCompositeNode *topNode, const string &nodename)
{
  // write itself since this class is fine with saving by root
  PdbParameterMap *nodeparams = findNode::getClass<PdbParameterMap>(topNode,nodename);
  if (!nodeparams)
    {
      nodeparams = new PdbParameterMap();
      PHIODataNode<PdbParameterMap> *newnode =  new PHIODataNode<PdbParameterMap>(nodeparams,nodename);
      topNode->addNode(newnode);
    }
  else
    {
      nodeparams->Reset(); // just clear previous content in case variables were deleted
    }
  CopyToPdbParameterMap(nodeparams);
  return;
}

int
PHG4Parameters::WriteToDB()
{
  PdbBankManager* bankManager = PdbBankManager::instance();
  PdbApplication *application = bankManager->getApplication();
  if (!application->startUpdate())
    {
      cout << PHWHERE << " Aborting, Database not writable" << endl;
      application->abort();
      exit(1);
    }

  //  Make a bank ID...
  PdbBankID bankID(0); // lets start at zero
  PHTimeStamp TStart(0);
  PHTimeStamp TStop(0xffffffff);

  string tablename = detname + "_geoparams";
  std::transform(tablename.begin(), tablename.end(), tablename.begin(), ::tolower);
  PdbCalBank *NewBank = bankManager->createBank("PdbParameterMapBank",
						bankID,
						"Geometry Parameters",
						TStart, TStop,
						tablename);
  if (NewBank)
    {
      NewBank->setLength(1);
      PdbParameterMap *myparm = (PdbParameterMap*)&NewBank->getEntry(0);
      CopyToPdbParameterMap(myparm);
      application->commit(NewBank);
      delete NewBank;
    }
  else
    {
      cout << PHWHERE " Committing to DB failed" << endl;
      return -1;
    }
  return 0;
}

int
PHG4Parameters::ReadFromDB()
{
  PdbBankManager* bankManager = PdbBankManager::instance();
  PdbApplication *application = bankManager->getApplication();
  if (!application->startRead())
    {
      cout << PHWHERE << " Aborting, Database not readable" << endl;
      application->abort();
      exit(1);
    }

  //  Make a bank ID...
  PdbBankID bankID(0); // lets start at zero
  PHTimeStamp TSearch(10);

  string tablename = detname + "_geoparams";
  std::transform(tablename.begin(), tablename.end(), tablename.begin(), ::tolower);
  PdbCalBank *NewBank = bankManager->fetchBank("PdbParameterMapBank",
						bankID,
					       tablename,
					       TSearch);
  if (NewBank)
    {
      PdbParameterMap *myparm = (PdbParameterMap*)&NewBank->getEntry(0);
      FillFrom(myparm);
      delete NewBank;
    }
  else
    {
      cout << PHWHERE " Reading from DB failed" << endl;
      return -1;
    }
  return 0;
}

int
PHG4Parameters::WriteToFile(const string &extension, const string &dir)
{
  ostringstream fnamestream;
  PdbBankID bankID(0); // lets start at zero
  PHTimeStamp TStart(0);
  PHTimeStamp TStop(0xffffffff);
  fnamestream << dir;
  // add / if directory lacks ending /
  if (*(dir.rbegin()) != '/')
    { 
      fnamestream << "/";
    }
  fnamestream << detname << "_geoparams" << "-" << bankID.getInternalValue() << "-" << TStart.getTics() << "-" << TStop.getTics() << "-" << time(0) << "." << extension;
  string fname = fnamestream.str();
  std::transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
  PdbParameterMap *myparm = new PdbParameterMap();
  CopyToPdbParameterMap(myparm);
  TFile *f = TFile::Open(fname.c_str(),"recreate");
  myparm->Write();
  delete f;
  cout << "sleeping 1 second to prevent duplicate inserttimes" << endl;
  sleep(1);
  return 0;
}

int
PHG4Parameters::ReadFromFile(const string &extension, const string &dir)
{
  PHTimeStamp TSearch(10);
  PdbBankID bankID(0);
  ostringstream fnamestream;
  fnamestream <<  detname << "_geoparams" << "-" << bankID.getInternalValue();
  string fileprefix = fnamestream.str();
  std::transform(fileprefix.begin(), fileprefix.end(), fileprefix.begin(), ::tolower);
  boost::filesystem::path targetDir(dir);

  boost::filesystem::recursive_directory_iterator iter(targetDir), eod;
  boost::char_separator<char> sep("-."); 
  map<unsigned int, string> calibfiles;
  BOOST_FOREACH(boost::filesystem::path const& i, make_pair(iter, eod))
    {
      if (is_regular_file(i))
	{
	  // boost leaf() gives the filename without path,
	  // this checks if the filename starts with fileprefix 
	  // (start pos of substring=0), if not coninue
	  string basename = i.filename().string();
	  if (basename.find(fileprefix))
	    {
	      continue;
	    }
	  // extension() contains the . - like .xml, so we
	  // just compare the extensions instead of !=
	  // and check that the size makes sense
	  if (i.extension().string().find(extension) == string::npos
	      || i.extension().string().size() != extension.size()+1)
	    {
	      continue;
	    }
	  boost::tokenizer<boost::char_separator<char> > tok(basename,sep);
	  boost::tokenizer<boost::char_separator<char> >::iterator iter = tok.begin();
	  ++iter; // that skips the file prefix excluding bank id
	  ++iter; // that skips the bank id we checked already as part of the filename
	  PHTimeStamp TStart(ConvertStringToUint(*iter));
	  if (TSearch < TStart)
	    {
	      continue; 
	    }
	  ++iter;
	  PHTimeStamp TStop(ConvertStringToUint(*iter));
	  if (TSearch >= TStop)
	    {
	      continue; 
	    }
	  ++iter;
	  calibfiles[ConvertStringToUint(*iter)] = i.string();

	}
    }
  if (calibfiles.empty())
    {
      cout << "No calibration file found" << endl;
      return -1;
    }
  cout << "Reading from File: " << (calibfiles.rbegin())->second << endl;
  string fname = (calibfiles.rbegin())->second;
  PdbParameterMap *myparm = new PdbParameterMap();
  TFile *f = TFile::Open(fname.c_str());
  myparm = (PdbParameterMap *) f->Get("PdbParameterMap");
  FillFrom(myparm);
  delete f;
  delete myparm;
  return 0;
}

void
PHG4Parameters::CopyToPdbParameterMap(PdbParameterMap *myparm)
{
  for (map<const string, double>::const_iterator iter = doubleparams.begin(); iter != doubleparams.end(); ++iter)
    {
      myparm->set_double_param(iter->first, iter->second);
    }
  for (map<const string, int>::const_iterator iter = intparams.begin(); iter != intparams.end(); ++iter)
    {
      myparm->set_int_param(iter->first, iter->second);
    }
  for (map<const string, string>::const_iterator iter = stringparams.begin(); iter != stringparams.end(); ++iter)
    {
      myparm->set_string_param(iter->first, iter->second);
    }
}

unsigned int
PHG4Parameters::ConvertStringToUint(const std::string &str) const
{
  unsigned int tics;
try
  {
    tics = boost::lexical_cast<unsigned int>(str);
  }
catch ( boost::bad_lexical_cast const& )
  {
    cout << "Cannot extract timestamp from " << str << endl;
    exit(1);
  }
 return tics;
}
