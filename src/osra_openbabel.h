/******************************************************************************
 OSRA: Optical Structure Recognition

 This is a U.S. Government work (2007-2010) and is therefore not subject to
 copyright. However, portions of this work were obtained from a GPL or
 GPL-compatible source.
 Created by Igor Filippov, 2007-2010 (igorf@helix.nih.gov)

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 PARTICULAR PURPOSE.  See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
 St, Fifth Floor, Boston, MA 02110-1301, USA
 *****************************************************************************/

#include <string> // std::string
#include <map> // std::map
#include <vector> // std::vector

extern "C" {
#include <potracelib.h>
}

using namespace std;

// Header: osra_openbabel.h
//
// Defines types and functions for OSRA OpenBabel module.
//

// struct: atom_s
//      Contains information about perspective atom
struct atom_s
{
  // doubles: x, y
  //    coordinates within the image clip
  double x, y;
  // string: label
  //    atomic label
  string label;
  // int: n
  //    counter of created OBAtom objects in <create_molecule()>
  int n;
  // int: anum
  //    atomic number
  int anum;
  // pointer: curve
  //    pointer to the curve found by Potrace
  const potrace_path_t *curve;
  // bools: exists, corner, terminal
  //    atom exists, atom is at the corner (has two bonds leading to it), atom is a terminal atom
  bool exists, corner, terminal;
  // int: charge
  //    electric charge on the atom
  int charge;
};
// typedef: atom_t
//      defines atom_t type based on atom_s struct
typedef struct atom_s atom_t;

// struct: bond_s
//      contains information about perspective bond between two atoms
struct bond_s
{
  // ints: a, b, type
  //    starting atom, ending atom, bond type (single/doouble/triple)
  int a, b, type;
  // pointer: curve
  //    pointer to the curve found by Potrace
  const potrace_path_t *curve;
  // bools: exists, hash, wedge, up, down, Small, arom
  //    bond existence and type flags
  bool exists;
  bool hash;
  bool wedge;
  bool up;
  bool down;
  bool Small;
  bool arom;
  // bool: conjoined
  //    true for a double bond which is joined at one end on the image
  bool conjoined;
};
// typedef: bond_t
//      defines bond_t type based on bond_s struct
typedef struct bond_s bond_t;

// struct: point_s
//      a point of the image, used by image segmentation routines
struct point_s
{
  // int: x,y
  //    coordinates of the image point
  int x, y;
};
// typedef: point_t
//      defines point_t type based on point_s struct
typedef struct point_s point_t;

// struct: box_s
//      encompassing box structure for image segmentation
struct box_s
{
  // int: x1, y1, x2, y2
  //    coordinates of top-left and bottom-right corners
  int x1, y1, x2, y2;
  // array: c
  //    vector of points in the box
  vector<point_t> c;
};
// typedef: box_t
//      defines box_t type based on box_s struct
typedef struct box_s box_t;

//struct: molecule_statistics_s
//      contains the statistical information about molecule used for analysis of recognition accuracy
struct molecule_statistics_s
{
  // int: rotors
  //    number of rotors in molecule
  int rotors;
  // int: num_fragments
  //    number of contiguous fragments in molecule
  int fragments;
  // int: rings56
  //    accumulated number of 5- and 6- rings in molecule
  int rings56;
};

// typedef: molecule_statistics_t
//      defines molecule_statistics_t type based on molecule_statistics_s struct
typedef struct molecule_statistics_s molecule_statistics_t;

//
// Section: Functions
//

// Function: caclulate_molecule_statistics()
//
// Converts vectors of atoms and bonds into a molecular object and calculates the molecule statistics.
// Note: this function changes the atoms!
//
// Parameters:
//      atom - vector of <atom_s> atoms
//      bond - vector of <bond_s> bonds
//      n_bond - total number of bonds
//      avg_bond_length - average bond length as measured from the image (to be included into output if provided)
//      superatom - dictionary of superatom labels mapped to SMILES
//
//  Returns:
//      calculated molecule statistics
molecule_statistics_t caclulate_molecule_statistics(vector<atom_t> &atom, const vector<bond_t> &bond, int n_bond,
    double avg_bond_length, const map<string, string> &superatom);

// Function: get_formatted_structure()
//
// Converts vectors of atoms and bonds into a molecular object and encodes the molecular into a text presentation (SMILES, MOL file, ...),
// specified by given format.
//
// Parameters:
//      atom - vector of <atom_s> atoms
//      bond - vector of <bond_s> bonds
//      n_bond - total number of bonds
//      format - output format for molecular representation - i.e. SMI, SDF
//      embedded_format - output format to be embedded into SDF (is only valid if output format is SDF); the only embedded formats supported now are "inchi", "smi", and "can"
//      molecule_statistics - the molecule statistics (returned to the caller)
//      confidence - confidence score (returned to the caller)
//      show_confidence - toggles confidence score inclusion into output
//      avg_bond_length - average bond length as measured from the image
//      scaled_avg_bond_length - average bond length scaled to the original resolution of the image
//      show_avg_bond_length - toggles average bond length inclusion into output
//      resolution - resolution at which image is being processed in DPI (to be included into output if provided)
//      page - page number (to be included into output if provided)
//      surrounding_box - the coordinates of surrounding image box that contains the structure (to be included into output if provided)
//      superatom - dictionary of superatom labels mapped to SMILES
//
//  Returns:
//      string containing SMILES, SDF or other representation of the molecule
const string get_formatted_structure(vector<atom_t> &atom, const vector<bond_t> &bond, int n_bond, const string &format, const string &second_format, molecule_statistics_t &molecule_statistics,
                                     double &confidence, bool show_confidence, double avg_bond_length, double scaled_avg_bond_length, bool show_avg_bond_length, const int * const resolution,
                                     const int * const page, const box_t * const surrounding_box, const map<string, string> &superatom);

