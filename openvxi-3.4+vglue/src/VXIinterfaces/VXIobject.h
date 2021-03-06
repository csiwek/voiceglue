
/****************License************************************************
 * Vocalocity OpenVXI
 * Copyright (C) 2004-2005 by Vocalocity, Inc. All Rights Reserved.
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
 * Vocalocity, the Vocalocity logo, and VocalOS are trademarks or 
 * registered trademarks of Vocalocity, Inc. 
 * OpenVXI is a trademark of Scansoft, Inc. and used under license 
 * by Vocalocity.
 ***********************************************************************/

#ifndef _VXIOBJECT_H
#define _VXIOBJECT_H

#include "VXItypes.h"
#include "VXIvalue.h"

#include "VXIheaderPrefix.h"
#ifdef VXIOBJECT_EXPORTS
#define VXIOBJECT_API SYMBOL_EXPORT_DECL
#else
#define VXIOBJECT_API SYMBOL_IMPORT_DECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * \defgroup VXIobject Object Interface
  *
  * Abstract interface for VoiceXML object functionality that allows
  * integrators to define VoiceXML language extensions that can be
  * executed by applications through the VoiceXML object element.
  * These objects can provide almost any extended functionality that
  * is desired.<p>
  */

/*@{*/

/**
 * @name Object Properties
 * Keys identifying properties in VXIMap for Execute( ) and Validate( )
 *
 * Note that VXIinet properties should also be included, these are
 * simply passed through to VXIinet for fetches as-is.
 */
/*@{*/
#define OBJECT_CLASS_ID        L"classid"       /* VXIString */
#define OBJECT_CODE_BASE       L"codebase"      /* VXIString */
#define OBJECT_CODE_TYPE       L"codetype"      /* VXIString */
#define OBJECT_DATA            L"data"          /* VXIString */
#define OBJECT_TYPE            L"type"          /* VXIString */
#define OBJECT_ARCHIVE         L"archive"       /* VXIString */
/*@}*/

/* Property defaults */
#define OBJECT_CLASS_ID_DEFAULT             L"" /* No default */
#define OBJECT_CODE_BASE_DEFAULT            L"" /* No base URI */
#define OBJECT_CODE_TYPE_DEFAULT            L"" /* Use OBJECT_TYPE */
#define OBJECT_DATA_DEFAULT                 L"" /* No data */
#define OBJECT_TYPE_DEFAULT                 L"" /* No data */
#define OBJECT_ARCHIVE_DEFAULT              L"" /* No archive */

/**
 * @name &lt;param&gt; keys
 * Keys identifying properties in the VXIMap for an individual parameter
 * (&lt;param&gt; element) in cases where "valuetype" is not "data".
 */
/*@{*/
#define OBJECT_VALUE           L"value"         /* VXIString */
#define OBJECT_VALUETYPE       L"valuetype"     /* VXIString */
#define OBJECT_TYPE            L"type"          /* VXIString */
/*@}*/
  
typedef enum VXIobjResult { 
  /** Fatal error, terminate call    */
  VXIobj_RESULT_FATAL_ERROR       =  -100, 
  /** I/O error                      */
  VXIobj_RESULT_IO_ERROR           =   -8,
  /** Out of memory                  */
  VXIobj_RESULT_OUT_OF_MEMORY      =   -7, 
  /** System error, out of service   */
  VXIobj_RESULT_SYSTEM_ERROR       =   -6, 
  /** Errors from platform services  */
  VXIobj_RESULT_PLATFORM_ERROR     =   -5, 
  /** Return buffer too small        */
  VXIobj_RESULT_BUFFER_TOO_SMALL   =   -4, 
  /** Property name is not valid    */
  VXIobj_RESULT_INVALID_PROP_NAME  =   -3, 
  /** Property value is not valid   */
  VXIobj_RESULT_INVALID_PROP_VALUE =   -2, 
  /** Invalid function argument      */
  VXIobj_RESULT_INVALID_ARGUMENT   =   -1, 
  /** Success.  Note that Success is defined as 0 and that all
      critical errors are less than 0 and all non critical errors are
      greater than 0.                */
  VXIobj_RESULT_SUCCESS            =    0,
  /** Normal failure, nothing logged */
  VXIobj_RESULT_FAILURE            =    1,
  /** Non-fatal non-specific error   */
  VXIobj_RESULT_NON_FATAL_ERROR    =    2, 
  /** Operation is not supported     */
  VXIobj_RESULT_UNSUPPORTED        =  100
} VXIobjResult;

/** 
 * Abstract interface for VoiceXML object functionality that allows
 * integrators to define VoiceXML language extensions that can be
 * executed by applications through the VoiceXML object element.
 * These objects can provide almost any extended functionality that
 * is desired.<p>
 *
 * There is one object interface per thread/line.  
 */
typedef struct VXIobjectInterface {
  /**
   * Get the VXI interface version implemented
   *
   * @return  VXIint32 for the version number. The high high word is 
   *          the major version number, the low word is the minor version 
   *          number, using the native CPU/OS byte order. The current
   *          version is VXI_CURRENT_VERSION as defined in VXItypes.h.
   */ 
  VXIint32 (*GetVersion)(void);
  
  /**
   * Get the name of the implementation
   *
   * @return  Implementation defined string that must be different from
   *          all other implementations. The recommended name is one
   *          where the interface name is prefixed by the implementator's
   *          Internet address in reverse order, such as com.xyz.rec for
   *          VXIobject from xyz.com. This is similar to how VoiceXML 1.0
   *          recommends defining application specific error types.
   */
  const VXIchar* (*GetImplementationName)(void);
  
  /**
   * Execute an object
   *
   * @param properties  [IN] Map containing properties and attributes for
   *                      the &lt;object&gt; as specified above.
   * @param parameters  [IN] Map containing parameters for the &lt;object&gt; as
   *                      specified by the VoiceXML &lt;param&gt; tag. The keys
   *                      of the map correspond to the parameter name ("name"
   *                      attribute) while the value of each key corresponds
   *                      to a VXIValue based type.
   *
   *                      For each parameter, any ECMAScript expressions are
   *                      evaluated by the interpreter. Then if the "valuetype"
   *                      attribute is set to "ref" the parameter value is
   *                      packaged into a VXIMap with three properties:
   *
   *                      OBJECT_VALUE:       actual parameter value
   *                      OBJECT_VALUETYPE:   "valuetype" attribute value
   *                      OBJECT_TYPE:        "type" attribute value
   *
   *                      Otherwise a primitive VXIValue based type will
   *                      be used to specify the value.
   * @param result      [OUT] Return value for the &lt;object&gt; execution, this
   *                      is allocated on success, the caller is responsible
   *                      for destroying the returned value by calling 
   *                      VXIValueDestroy( ). The object's field variable
   *                      will be set to this value.
   *
   * @return        VXIobj_RESULT_SUCCESS on success,
   *                VXIobj_RESULT_NON_FATAL_ERROR on error, 
   *                VXIobj_RESULT_UNSUPPORTED for unsupported object types
   *                 (this will cause interpreter to throw the correct event)
   */
  VXIobjResult (*Execute)(struct VXIobjectInterface *pThis,
			  const VXIMap              *properties,
			  const VXIMap              *parameters,
			  VXIValue                 **result);

 /**
   * Validate an object, performing validity checks without execution
   *
   * @param properties  [IN] Map containing properties and attributes for
   *                      the &lt;object&gt; as specified in the VoiceXML
   *                      specification except that "expr" and "cond" are
   *                      always omitted (are handled by the interpreter).
   * @param parameters  [IN] Map containing parameters for the &lt;object&gt; as
   *                      specified by the VoiceXML &lt;param&gt; tag. The keys
   *                      of the map correspond to the parameter name ("name"
   *                      attribute) while the value of each key corresponds
   *                      to a VXIValue based type. See Execute( ) above 
   *                      for details.
   *
   * @return        VXIobj_RESULT_SUCCESS on success,
   *                VXIobj_RESULT_NON_FATAL_ERROR on error, 
   *                VXIobj_RESULT_UNSUPPORTED for unsupported object types
   *                 (this will cause interpreter to throw the correct event)
   */
  VXIobjResult (*Validate)(struct VXIobjectInterface *pThis,
			   const VXIMap              *properties,
			   const VXIMap              *parameters);

} VXIobjectInterface;

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
