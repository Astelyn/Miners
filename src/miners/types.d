// Copyright © 2011, Jakob Bornecrantz.  All rights reserved.
// See copyright notice in src/charge/charge.d (GPLv2 only).
module miners.types;


/**
 * Used to control which mesh type that the terrain build.
 */
enum TerrainBuildTypes {
	RigidMesh,
	CompactMesh,
}


/**
 * Struct to hold information about a block.
 */
struct Block
{
	ubyte type;        /**< [0 - 255] */
	ubyte meta;        /**< [0 - 16] Mening is dependant on type */
	ubyte sunlight;    /**< [0 - 16] */
	ubyte torchlight;  /**< [0 - 16] */
}


/**
 * Information needed to connect to a classic server.
 */
class ClassicServerInfo
{
	string webName; /**< Name as given by the webpage. */
	string webId; /**< Id on the webpage */

	string hostname; /**< Hostname to connect to */
	ushort port; /**< Port number to connect to */
	string username; /**< Username used in slat */
	string verificationKey; /**< Retrived from webpage */
}
