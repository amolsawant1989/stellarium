/*
 * Stellarium
 * Copyright (C) 2002 Fabien Chereau
 * Copyright (C) 2011 Alexander Wolf
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _NEBULA_HPP_
#define _NEBULA_HPP_

#include <QString>
#include "StelObject.hpp"
#include "StelTranslator.hpp"
#include "StelTextureTypes.hpp"

//#define GEN_BIN_CATALOG

class StelPainter;
class QDataStream;

// Updated Nebula class based on catalogue of Wolfgang Steinicke
class Nebula : public StelObject
{
friend class NebulaMgr;
public:
	Nebula();
	~Nebula();

	//! Nebula support the following InfoStringGroup flags:
	//! - Name
	//! - CatalogNumber
	//! - Magnitude
	//! - RaDec
	//! - AltAzi
	//! - Distance
	//! - Size
	//! - Extra1 (contains the Nebula type, which might be "Galaxy", "Cluster" or similar)
	//! - PlainText
	//! @param core the StelCore object
	//! @param flags a set of InfoStringGroup items to include in the return value.
	//! @return a QString containing an HMTL encoded description of the Nebula.
	virtual QString getInfoString(const StelCore *core, const InfoStringGroup& flags) const;
	virtual QString getType() const {return "Nebula";}
	virtual Vec3d getJ2000EquatorialPos(const StelCore*) const {return XYZ;}
	virtual double getCloseViewFov(const StelCore* core = NULL) const;
	virtual float getVMagnitude(const StelCore* core = NULL) const {Q_UNUSED(core); return mag;}
	virtual float getSelectPriority(const StelCore* core) const;
	virtual Vec3f getInfoColor() const;
	virtual QString getNameI18n() const {return nameI18;}
	virtual QString getEnglishName() const {return englishName;}
	virtual double getAngularSize(const StelCore*) const {return angularSize*0.5;}
	virtual SphericalRegionP getRegion() const {return pointRegion;}

	// Methods specific to Nebula
	void setLabelColor(const Vec3f& v) {labelColor = v;}
	void setCircleColor(const Vec3f& v) {circleColor = v;}

	//! Get the printable nebula Type.
	//! @return the nebula type code.
	QString getTypeString() const;

private:
	friend struct DrawNebulaFuncObject;
	
	//! @enum NebulaType Nebula types
	enum NebulaType
	{
		NebGx,     //!< Galaxy
		NebOpenC,  //!< Open star cluster
		NebGlobC,  //!< Globular star cluster, usually in the Milky Way Galaxy
		NebN,      //!< Bright emission or reflection nebula [deprecated]
		NebPNe,    //!< Planetary nebula
		NebDn,     //!< ???
		NebIg,     //!< ???
		NebCn,     //!< Cluster associated with nebulosity [deprecated]
		NebUnknown, //!< Unknown type
		// NEW types added for W. Steinicke's catalogue
		NebGNe,		//!< Galactic nebula
		NebEmis,	//!< Emission nebula
		NebCopy,	//!< WARNING: repeated object
		NebInNGC,	//!< WARNING: object already exists in NGC catalogue
		NebStar		//!< Nebula is actually a star
#if 0
		NebGx=0,     //!< Galaxy
		NebOc=1,     //!< Open star cluster
		NebGc=2,     //!< Globular star cluster, usually in the Milky Way Galaxy
		NebN=3,      //!< Bright emission or reflection nebula
		NebPn=4,     //!< Planetary nebula
		NebDn=5,     //!< ??? 
		NebIg=6,     //!< ??? 
		NebCn=7,     //!< Cluster associated with nebulosity
		NebUnknown=8 //!< Unknown type
#endif // MERGE-SOURCE
	};

	//! Translate nebula name using the passed translator
	void translateName(StelTranslator& trans) {nameI18 = trans.qtranslate(englishName);}

	void readNGC(QDataStream& in);
	void readExtendedNGC(QDataStream& in);
	bool readNGC(QString& record);
	void writeExtendedNGC(QDataStream& out);
	void readIdentifiers(const QString& record);
	void parseRecord(const QString& record, int idx);
	
	// debug only!
	void debugNGC(float ra);

	
	void drawLabel(StelPainter& sPainter, float maxMagLabel);
	void drawHints(StelPainter& sPainter, float maxMagHints);

	unsigned int M_nb;              // Messier Catalog number
	unsigned int NGC_nb;            // New General Catalog number
	unsigned int IC_nb;             // Index Catalog number
	QString englishName;            // English name
	QString nameI18;                // Nebula name
	float mag;                      // Apparent magnitude
	float angularSize;              // Angular size in degree
	Vec3d XYZ;                      // Cartesian equatorial position
        Vec3d XYZ2;                      // FIXME: Cartesian equatorial position of direction N
        Vec3d XY;                       // Store temporary 2D position
        Vec3d XY2;                       // FIXME: Store temporary 2D position
        NebulaType nType;

	bool bNGCObject;
	float BminusV;
	int PGC_nb;
	QString altDesig1;			//!< alternative designation 1

	SphericalRegionP pointRegion;

	// Additional data from W. Steinicke's catalogue
	bool bDreyerObject;			//!< is it in original Dreyer catalogue?
	QString constellationAbbr;	//!< constellation abbrev.
	float magB;					//!< blue magnitude
	float magV;					//!< visual magnitude
	float SBrightness;			//!< surface brightness (mag/arcmin2)
	float sizeX;				//!< major axis (arcmin)
	float sizeY;				//!< minor axis (arcmin)
	float PAdeg;				//!< principal angle (range 0..360 degrees)
	QString hubbleType;			//!< Hubble type for galaxies
    float redshift;
    float dist;
    float distz;

#if defined(GEN_BIN_CATALOG)
// for creating binary file
	float _ra, _dec;
#endif
// Stellarium properties
	static StelTextureSP texCircle;   // The symbolic circle texture
	static StelTextureSP texOpenCluster;
	static StelTextureSP texGlobularCluster;
	static StelTextureSP texPlanetNebula;
	static float hintsBrightness;

	static Vec3f labelColor, circleColor;
	static float circleScale;       // Define the scaling of the hints circle
};

#endif // _NEBULA_HPP_

