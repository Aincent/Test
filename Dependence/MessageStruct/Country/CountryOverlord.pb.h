// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Country/CountryOverlord.proto

#ifndef PROTOBUF_Country_2fCountryOverlord_2eproto__INCLUDED
#define PROTOBUF_Country_2fCountryOverlord_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace CountryOverlord {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

class AckReqCountryOverlordMapID;
class ReqCountryParticipationQualification;
class AckCountryParticipationQualification;
class ParticipationQualification;
class OverlordWarData;
class WarData;

// ===================================================================

class AckReqCountryOverlordMapID : public ::google::protobuf::Message {
 public:
  AckReqCountryOverlordMapID();
  virtual ~AckReqCountryOverlordMapID();

  AckReqCountryOverlordMapID(const AckReqCountryOverlordMapID& from);

  inline AckReqCountryOverlordMapID& operator=(const AckReqCountryOverlordMapID& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AckReqCountryOverlordMapID& default_instance();

  void Swap(AckReqCountryOverlordMapID* other);

  // implements Message ----------------------------------------------

  AckReqCountryOverlordMapID* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AckReqCountryOverlordMapID& from);
  void MergeFrom(const AckReqCountryOverlordMapID& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int64 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int64 id() const;
  inline void set_id(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:CountryOverlord.AckReqCountryOverlordMapID)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static AckReqCountryOverlordMapID* default_instance_;
};
// -------------------------------------------------------------------

class ReqCountryParticipationQualification : public ::google::protobuf::Message {
 public:
  ReqCountryParticipationQualification();
  virtual ~ReqCountryParticipationQualification();

  ReqCountryParticipationQualification(const ReqCountryParticipationQualification& from);

  inline ReqCountryParticipationQualification& operator=(const ReqCountryParticipationQualification& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqCountryParticipationQualification& default_instance();

  void Swap(ReqCountryParticipationQualification* other);

  // implements Message ----------------------------------------------

  ReqCountryParticipationQualification* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqCountryParticipationQualification& from);
  void MergeFrom(const ReqCountryParticipationQualification& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 country = 1;
  inline bool has_country() const;
  inline void clear_country();
  static const int kCountryFieldNumber = 1;
  inline ::google::protobuf::int32 country() const;
  inline void set_country(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:CountryOverlord.ReqCountryParticipationQualification)
 private:
  inline void set_has_country();
  inline void clear_has_country();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 country_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static ReqCountryParticipationQualification* default_instance_;
};
// -------------------------------------------------------------------

class AckCountryParticipationQualification : public ::google::protobuf::Message {
 public:
  AckCountryParticipationQualification();
  virtual ~AckCountryParticipationQualification();

  AckCountryParticipationQualification(const AckCountryParticipationQualification& from);

  inline AckCountryParticipationQualification& operator=(const AckCountryParticipationQualification& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AckCountryParticipationQualification& default_instance();

  void Swap(AckCountryParticipationQualification* other);

  // implements Message ----------------------------------------------

  AckCountryParticipationQualification* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AckCountryParticipationQualification& from);
  void MergeFrom(const AckCountryParticipationQualification& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 country = 1;
  inline bool has_country() const;
  inline void clear_country();
  static const int kCountryFieldNumber = 1;
  inline ::google::protobuf::int32 country() const;
  inline void set_country(::google::protobuf::int32 value);

  // repeated .CountryOverlord.ParticipationQualification pqinfo = 2;
  inline int pqinfo_size() const;
  inline void clear_pqinfo();
  static const int kPqinfoFieldNumber = 2;
  inline const ::CountryOverlord::ParticipationQualification& pqinfo(int index) const;
  inline ::CountryOverlord::ParticipationQualification* mutable_pqinfo(int index);
  inline ::CountryOverlord::ParticipationQualification* add_pqinfo();
  inline const ::google::protobuf::RepeatedPtrField< ::CountryOverlord::ParticipationQualification >&
      pqinfo() const;
  inline ::google::protobuf::RepeatedPtrField< ::CountryOverlord::ParticipationQualification >*
      mutable_pqinfo();

  // @@protoc_insertion_point(class_scope:CountryOverlord.AckCountryParticipationQualification)
 private:
  inline void set_has_country();
  inline void clear_has_country();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::CountryOverlord::ParticipationQualification > pqinfo_;
  ::google::protobuf::int32 country_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static AckCountryParticipationQualification* default_instance_;
};
// -------------------------------------------------------------------

class ParticipationQualification : public ::google::protobuf::Message {
 public:
  ParticipationQualification();
  virtual ~ParticipationQualification();

  ParticipationQualification(const ParticipationQualification& from);

  inline ParticipationQualification& operator=(const ParticipationQualification& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ParticipationQualification& default_instance();

  void Swap(ParticipationQualification* other);

  // implements Message ----------------------------------------------

  ParticipationQualification* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ParticipationQualification& from);
  void MergeFrom(const ParticipationQualification& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 rank = 1;
  inline bool has_rank() const;
  inline void clear_rank();
  static const int kRankFieldNumber = 1;
  inline ::google::protobuf::int32 rank() const;
  inline void set_rank(::google::protobuf::int32 value);

  // optional string charname = 2;
  inline bool has_charname() const;
  inline void clear_charname();
  static const int kCharnameFieldNumber = 2;
  inline const ::std::string& charname() const;
  inline void set_charname(const ::std::string& value);
  inline void set_charname(const char* value);
  inline void set_charname(const char* value, size_t size);
  inline ::std::string* mutable_charname();
  inline ::std::string* release_charname();
  inline void set_allocated_charname(::std::string* charname);

  // optional int32 lv = 3;
  inline bool has_lv() const;
  inline void clear_lv();
  static const int kLvFieldNumber = 3;
  inline ::google::protobuf::int32 lv() const;
  inline void set_lv(::google::protobuf::int32 value);

  // optional int32 contribute = 4;
  inline bool has_contribute() const;
  inline void clear_contribute();
  static const int kContributeFieldNumber = 4;
  inline ::google::protobuf::int32 contribute() const;
  inline void set_contribute(::google::protobuf::int32 value);

  // optional int32 job = 5;
  inline bool has_job() const;
  inline void clear_job();
  static const int kJobFieldNumber = 5;
  inline ::google::protobuf::int32 job() const;
  inline void set_job(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:CountryOverlord.ParticipationQualification)
 private:
  inline void set_has_rank();
  inline void clear_has_rank();
  inline void set_has_charname();
  inline void clear_has_charname();
  inline void set_has_lv();
  inline void clear_has_lv();
  inline void set_has_contribute();
  inline void clear_has_contribute();
  inline void set_has_job();
  inline void clear_has_job();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* charname_;
  ::google::protobuf::int32 rank_;
  ::google::protobuf::int32 lv_;
  ::google::protobuf::int32 contribute_;
  ::google::protobuf::int32 job_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static ParticipationQualification* default_instance_;
};
// -------------------------------------------------------------------

class OverlordWarData : public ::google::protobuf::Message {
 public:
  OverlordWarData();
  virtual ~OverlordWarData();

  OverlordWarData(const OverlordWarData& from);

  inline OverlordWarData& operator=(const OverlordWarData& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const OverlordWarData& default_instance();

  void Swap(OverlordWarData* other);

  // implements Message ----------------------------------------------

  OverlordWarData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const OverlordWarData& from);
  void MergeFrom(const OverlordWarData& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .CountryOverlord.WarData data = 1;
  inline int data_size() const;
  inline void clear_data();
  static const int kDataFieldNumber = 1;
  inline const ::CountryOverlord::WarData& data(int index) const;
  inline ::CountryOverlord::WarData* mutable_data(int index);
  inline ::CountryOverlord::WarData* add_data();
  inline const ::google::protobuf::RepeatedPtrField< ::CountryOverlord::WarData >&
      data() const;
  inline ::google::protobuf::RepeatedPtrField< ::CountryOverlord::WarData >*
      mutable_data();

  // @@protoc_insertion_point(class_scope:CountryOverlord.OverlordWarData)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::CountryOverlord::WarData > data_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static OverlordWarData* default_instance_;
};
// -------------------------------------------------------------------

class WarData : public ::google::protobuf::Message {
 public:
  WarData();
  virtual ~WarData();

  WarData(const WarData& from);

  inline WarData& operator=(const WarData& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const WarData& default_instance();

  void Swap(WarData* other);

  // implements Message ----------------------------------------------

  WarData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const WarData& from);
  void MergeFrom(const WarData& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string charname = 1;
  inline bool has_charname() const;
  inline void clear_charname();
  static const int kCharnameFieldNumber = 1;
  inline const ::std::string& charname() const;
  inline void set_charname(const ::std::string& value);
  inline void set_charname(const char* value);
  inline void set_charname(const char* value, size_t size);
  inline ::std::string* mutable_charname();
  inline ::std::string* release_charname();
  inline void set_allocated_charname(::std::string* charname);

  // optional int32 killer = 2;
  inline bool has_killer() const;
  inline void clear_killer();
  static const int kKillerFieldNumber = 2;
  inline ::google::protobuf::int32 killer() const;
  inline void set_killer(::google::protobuf::int32 value);

  // optional int32 integral = 3;
  inline bool has_integral() const;
  inline void clear_integral();
  static const int kIntegralFieldNumber = 3;
  inline ::google::protobuf::int32 integral() const;
  inline void set_integral(::google::protobuf::int32 value);

  // optional string clanname = 4;
  inline bool has_clanname() const;
  inline void clear_clanname();
  static const int kClannameFieldNumber = 4;
  inline const ::std::string& clanname() const;
  inline void set_clanname(const ::std::string& value);
  inline void set_clanname(const char* value);
  inline void set_clanname(const char* value, size_t size);
  inline ::std::string* mutable_clanname();
  inline ::std::string* release_clanname();
  inline void set_allocated_clanname(::std::string* clanname);

  // optional int32 country = 5;
  inline bool has_country() const;
  inline void clear_country();
  static const int kCountryFieldNumber = 5;
  inline ::google::protobuf::int32 country() const;
  inline void set_country(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:CountryOverlord.WarData)
 private:
  inline void set_has_charname();
  inline void clear_has_charname();
  inline void set_has_killer();
  inline void clear_has_killer();
  inline void set_has_integral();
  inline void clear_has_integral();
  inline void set_has_clanname();
  inline void clear_has_clanname();
  inline void set_has_country();
  inline void clear_has_country();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* charname_;
  ::google::protobuf::int32 killer_;
  ::google::protobuf::int32 integral_;
  ::std::string* clanname_;
  ::google::protobuf::int32 country_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_AssignDesc_Country_2fCountryOverlord_2eproto();
  friend void protobuf_ShutdownFile_Country_2fCountryOverlord_2eproto();

  void InitAsDefaultInstance();
  static WarData* default_instance_;
};
// ===================================================================


// ===================================================================

// AckReqCountryOverlordMapID

// required int64 id = 1;
inline bool AckReqCountryOverlordMapID::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AckReqCountryOverlordMapID::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AckReqCountryOverlordMapID::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AckReqCountryOverlordMapID::clear_id() {
  id_ = GOOGLE_LONGLONG(0);
  clear_has_id();
}
inline ::google::protobuf::int64 AckReqCountryOverlordMapID::id() const {
  return id_;
}
inline void AckReqCountryOverlordMapID::set_id(::google::protobuf::int64 value) {
  set_has_id();
  id_ = value;
}

// -------------------------------------------------------------------

// ReqCountryParticipationQualification

// required int32 country = 1;
inline bool ReqCountryParticipationQualification::has_country() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqCountryParticipationQualification::set_has_country() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqCountryParticipationQualification::clear_has_country() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqCountryParticipationQualification::clear_country() {
  country_ = 0;
  clear_has_country();
}
inline ::google::protobuf::int32 ReqCountryParticipationQualification::country() const {
  return country_;
}
inline void ReqCountryParticipationQualification::set_country(::google::protobuf::int32 value) {
  set_has_country();
  country_ = value;
}

// -------------------------------------------------------------------

// AckCountryParticipationQualification

// required int32 country = 1;
inline bool AckCountryParticipationQualification::has_country() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AckCountryParticipationQualification::set_has_country() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AckCountryParticipationQualification::clear_has_country() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AckCountryParticipationQualification::clear_country() {
  country_ = 0;
  clear_has_country();
}
inline ::google::protobuf::int32 AckCountryParticipationQualification::country() const {
  return country_;
}
inline void AckCountryParticipationQualification::set_country(::google::protobuf::int32 value) {
  set_has_country();
  country_ = value;
}

// repeated .CountryOverlord.ParticipationQualification pqinfo = 2;
inline int AckCountryParticipationQualification::pqinfo_size() const {
  return pqinfo_.size();
}
inline void AckCountryParticipationQualification::clear_pqinfo() {
  pqinfo_.Clear();
}
inline const ::CountryOverlord::ParticipationQualification& AckCountryParticipationQualification::pqinfo(int index) const {
  return pqinfo_.Get(index);
}
inline ::CountryOverlord::ParticipationQualification* AckCountryParticipationQualification::mutable_pqinfo(int index) {
  return pqinfo_.Mutable(index);
}
inline ::CountryOverlord::ParticipationQualification* AckCountryParticipationQualification::add_pqinfo() {
  return pqinfo_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::CountryOverlord::ParticipationQualification >&
AckCountryParticipationQualification::pqinfo() const {
  return pqinfo_;
}
inline ::google::protobuf::RepeatedPtrField< ::CountryOverlord::ParticipationQualification >*
AckCountryParticipationQualification::mutable_pqinfo() {
  return &pqinfo_;
}

// -------------------------------------------------------------------

// ParticipationQualification

// required int32 rank = 1;
inline bool ParticipationQualification::has_rank() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ParticipationQualification::set_has_rank() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ParticipationQualification::clear_has_rank() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ParticipationQualification::clear_rank() {
  rank_ = 0;
  clear_has_rank();
}
inline ::google::protobuf::int32 ParticipationQualification::rank() const {
  return rank_;
}
inline void ParticipationQualification::set_rank(::google::protobuf::int32 value) {
  set_has_rank();
  rank_ = value;
}

// optional string charname = 2;
inline bool ParticipationQualification::has_charname() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ParticipationQualification::set_has_charname() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ParticipationQualification::clear_has_charname() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ParticipationQualification::clear_charname() {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    charname_->clear();
  }
  clear_has_charname();
}
inline const ::std::string& ParticipationQualification::charname() const {
  return *charname_;
}
inline void ParticipationQualification::set_charname(const ::std::string& value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void ParticipationQualification::set_charname(const char* value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void ParticipationQualification::set_charname(const char* value, size_t size) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ParticipationQualification::mutable_charname() {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  return charname_;
}
inline ::std::string* ParticipationQualification::release_charname() {
  clear_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = charname_;
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ParticipationQualification::set_allocated_charname(::std::string* charname) {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    delete charname_;
  }
  if (charname) {
    set_has_charname();
    charname_ = charname;
  } else {
    clear_has_charname();
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 lv = 3;
inline bool ParticipationQualification::has_lv() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ParticipationQualification::set_has_lv() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ParticipationQualification::clear_has_lv() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ParticipationQualification::clear_lv() {
  lv_ = 0;
  clear_has_lv();
}
inline ::google::protobuf::int32 ParticipationQualification::lv() const {
  return lv_;
}
inline void ParticipationQualification::set_lv(::google::protobuf::int32 value) {
  set_has_lv();
  lv_ = value;
}

// optional int32 contribute = 4;
inline bool ParticipationQualification::has_contribute() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ParticipationQualification::set_has_contribute() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ParticipationQualification::clear_has_contribute() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ParticipationQualification::clear_contribute() {
  contribute_ = 0;
  clear_has_contribute();
}
inline ::google::protobuf::int32 ParticipationQualification::contribute() const {
  return contribute_;
}
inline void ParticipationQualification::set_contribute(::google::protobuf::int32 value) {
  set_has_contribute();
  contribute_ = value;
}

// optional int32 job = 5;
inline bool ParticipationQualification::has_job() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ParticipationQualification::set_has_job() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ParticipationQualification::clear_has_job() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ParticipationQualification::clear_job() {
  job_ = 0;
  clear_has_job();
}
inline ::google::protobuf::int32 ParticipationQualification::job() const {
  return job_;
}
inline void ParticipationQualification::set_job(::google::protobuf::int32 value) {
  set_has_job();
  job_ = value;
}

// -------------------------------------------------------------------

// OverlordWarData

// repeated .CountryOverlord.WarData data = 1;
inline int OverlordWarData::data_size() const {
  return data_.size();
}
inline void OverlordWarData::clear_data() {
  data_.Clear();
}
inline const ::CountryOverlord::WarData& OverlordWarData::data(int index) const {
  return data_.Get(index);
}
inline ::CountryOverlord::WarData* OverlordWarData::mutable_data(int index) {
  return data_.Mutable(index);
}
inline ::CountryOverlord::WarData* OverlordWarData::add_data() {
  return data_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::CountryOverlord::WarData >&
OverlordWarData::data() const {
  return data_;
}
inline ::google::protobuf::RepeatedPtrField< ::CountryOverlord::WarData >*
OverlordWarData::mutable_data() {
  return &data_;
}

// -------------------------------------------------------------------

// WarData

// optional string charname = 1;
inline bool WarData::has_charname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void WarData::set_has_charname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void WarData::clear_has_charname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void WarData::clear_charname() {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    charname_->clear();
  }
  clear_has_charname();
}
inline const ::std::string& WarData::charname() const {
  return *charname_;
}
inline void WarData::set_charname(const ::std::string& value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void WarData::set_charname(const char* value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void WarData::set_charname(const char* value, size_t size) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* WarData::mutable_charname() {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  return charname_;
}
inline ::std::string* WarData::release_charname() {
  clear_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = charname_;
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void WarData::set_allocated_charname(::std::string* charname) {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    delete charname_;
  }
  if (charname) {
    set_has_charname();
    charname_ = charname;
  } else {
    clear_has_charname();
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 killer = 2;
inline bool WarData::has_killer() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void WarData::set_has_killer() {
  _has_bits_[0] |= 0x00000002u;
}
inline void WarData::clear_has_killer() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void WarData::clear_killer() {
  killer_ = 0;
  clear_has_killer();
}
inline ::google::protobuf::int32 WarData::killer() const {
  return killer_;
}
inline void WarData::set_killer(::google::protobuf::int32 value) {
  set_has_killer();
  killer_ = value;
}

// optional int32 integral = 3;
inline bool WarData::has_integral() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void WarData::set_has_integral() {
  _has_bits_[0] |= 0x00000004u;
}
inline void WarData::clear_has_integral() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void WarData::clear_integral() {
  integral_ = 0;
  clear_has_integral();
}
inline ::google::protobuf::int32 WarData::integral() const {
  return integral_;
}
inline void WarData::set_integral(::google::protobuf::int32 value) {
  set_has_integral();
  integral_ = value;
}

// optional string clanname = 4;
inline bool WarData::has_clanname() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void WarData::set_has_clanname() {
  _has_bits_[0] |= 0x00000008u;
}
inline void WarData::clear_has_clanname() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void WarData::clear_clanname() {
  if (clanname_ != &::google::protobuf::internal::kEmptyString) {
    clanname_->clear();
  }
  clear_has_clanname();
}
inline const ::std::string& WarData::clanname() const {
  return *clanname_;
}
inline void WarData::set_clanname(const ::std::string& value) {
  set_has_clanname();
  if (clanname_ == &::google::protobuf::internal::kEmptyString) {
    clanname_ = new ::std::string;
  }
  clanname_->assign(value);
}
inline void WarData::set_clanname(const char* value) {
  set_has_clanname();
  if (clanname_ == &::google::protobuf::internal::kEmptyString) {
    clanname_ = new ::std::string;
  }
  clanname_->assign(value);
}
inline void WarData::set_clanname(const char* value, size_t size) {
  set_has_clanname();
  if (clanname_ == &::google::protobuf::internal::kEmptyString) {
    clanname_ = new ::std::string;
  }
  clanname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* WarData::mutable_clanname() {
  set_has_clanname();
  if (clanname_ == &::google::protobuf::internal::kEmptyString) {
    clanname_ = new ::std::string;
  }
  return clanname_;
}
inline ::std::string* WarData::release_clanname() {
  clear_has_clanname();
  if (clanname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = clanname_;
    clanname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void WarData::set_allocated_clanname(::std::string* clanname) {
  if (clanname_ != &::google::protobuf::internal::kEmptyString) {
    delete clanname_;
  }
  if (clanname) {
    set_has_clanname();
    clanname_ = clanname;
  } else {
    clear_has_clanname();
    clanname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 country = 5;
inline bool WarData::has_country() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void WarData::set_has_country() {
  _has_bits_[0] |= 0x00000010u;
}
inline void WarData::clear_has_country() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void WarData::clear_country() {
  country_ = 0;
  clear_has_country();
}
inline ::google::protobuf::int32 WarData::country() const {
  return country_;
}
inline void WarData::set_country(::google::protobuf::int32 value) {
  set_has_country();
  country_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace CountryOverlord

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Country_2fCountryOverlord_2eproto__INCLUDED
