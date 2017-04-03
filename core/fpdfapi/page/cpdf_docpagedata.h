// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFAPI_PAGE_CPDF_DOCPAGEDATA_H_
#define CORE_FPDFAPI_PAGE_CPDF_DOCPAGEDATA_H_

#include <map>
#include <set>

#include "core/fpdfapi/page/cpdf_countedobject.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_string.h"

class CPDF_Dictionary;
class CPDF_Document;
class CPDF_Font;
class CPDF_FontEncoding;
class CPDF_IccProfile;
class CPDF_Image;
class CPDF_Object;
class CPDF_Stream;
class CPDF_StreamAcc;

class CPDF_DocPageData {
 public:
  explicit CPDF_DocPageData(CPDF_Document* pPDFDoc);
  ~CPDF_DocPageData();

  void Clear(bool bRelease = false);
  bool IsForceClear() const { return m_bForceClear; }

  CPDF_Font* GetFont(CPDF_Dictionary* pFontDict);
  CPDF_Font* GetStandardFont(const CFX_ByteString& fontName,
                             CPDF_FontEncoding* pEncoding);
  void ReleaseFont(const CPDF_Dictionary* pFontDict);

  CPDF_ColorSpace* GetColorSpace(CPDF_Object* pCSObj,
                                 const CPDF_Dictionary* pResources);
  CPDF_ColorSpace* GetCopiedColorSpace(CPDF_Object* pCSObj);
  void ReleaseColorSpace(const CPDF_Object* pColorSpace);

  CPDF_Pattern* GetPattern(CPDF_Object* pPatternObj,
                           bool bShading,
                           const CFX_Matrix& matrix);
  void ReleasePattern(const CPDF_Object* pPatternObj);

  CFX_RetainPtr<CPDF_Image> GetImage(uint32_t dwStreamObjNum);
  void MaybePurgeImage(uint32_t dwStreamObjNum);

  CFX_RetainPtr<CPDF_IccProfile> GetIccProfile(CPDF_Stream* pProfileStream);
  void MaybePurgeIccProfile(CPDF_Stream* pProfileStream);

  CPDF_StreamAcc* GetFontFileStreamAcc(CPDF_Stream* pFontStream);
  void ReleaseFontFileStreamAcc(const CPDF_Stream* pFontStream);

  CPDF_CountedColorSpace* FindColorSpacePtr(CPDF_Object* pCSObj) const;
  CPDF_CountedPattern* FindPatternPtr(CPDF_Object* pPatternObj) const;

 private:
  using CPDF_CountedFont = CPDF_CountedObject<CPDF_Font>;
  using CPDF_CountedStreamAcc = CPDF_CountedObject<CPDF_StreamAcc>;

  CPDF_ColorSpace* GetColorSpaceImpl(CPDF_Object* pCSObj,
                                     const CPDF_Dictionary* pResources,
                                     std::set<CPDF_Object*>* pVisited);

  CPDF_Document* const m_pPDFDoc;
  bool m_bForceClear;
  std::map<CFX_ByteString, CPDF_Stream*> m_HashProfileMap;
  std::map<const CPDF_Object*, CPDF_CountedColorSpace*> m_ColorSpaceMap;
  std::map<const CPDF_Stream*, CPDF_CountedStreamAcc*> m_FontFileMap;
  std::map<const CPDF_Dictionary*, CPDF_CountedFont*> m_FontMap;
  std::map<const CPDF_Stream*, CFX_RetainPtr<CPDF_IccProfile>> m_IccProfileMap;
  std::map<uint32_t, CFX_RetainPtr<CPDF_Image>> m_ImageMap;
  std::map<const CPDF_Object*, CPDF_CountedPattern*> m_PatternMap;
};

#endif  // CORE_FPDFAPI_PAGE_CPDF_DOCPAGEDATA_H_
