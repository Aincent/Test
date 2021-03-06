// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CharLogin/MoneyShop.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "CharLogin/MoneyShop.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace MoneyShopInfo {

namespace {

const ::google::protobuf::Descriptor* AckMoneyShopInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AckMoneyShopInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* AckExchangeMoney_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AckExchangeMoney_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_CharLogin_2fMoneyShop_2eproto() {
  protobuf_AddDesc_CharLogin_2fMoneyShop_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "CharLogin/MoneyShop.proto");
  GOOGLE_CHECK(file != NULL);
  AckMoneyShopInfo_descriptor_ = file->message_type(0);
  static const int AckMoneyShopInfo_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckMoneyShopInfo, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckMoneyShopInfo, freenum_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckMoneyShopInfo, costnum_),
  };
  AckMoneyShopInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AckMoneyShopInfo_descriptor_,
      AckMoneyShopInfo::default_instance_,
      AckMoneyShopInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckMoneyShopInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckMoneyShopInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AckMoneyShopInfo));
  AckExchangeMoney_descriptor_ = file->message_type(1);
  static const int AckExchangeMoney_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, money_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, freenum_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, costnum_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, critvalue_),
  };
  AckExchangeMoney_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AckExchangeMoney_descriptor_,
      AckExchangeMoney::default_instance_,
      AckExchangeMoney_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AckExchangeMoney, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AckExchangeMoney));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_CharLogin_2fMoneyShop_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AckMoneyShopInfo_descriptor_, &AckMoneyShopInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AckExchangeMoney_descriptor_, &AckExchangeMoney::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_CharLogin_2fMoneyShop_2eproto() {
  delete AckMoneyShopInfo::default_instance_;
  delete AckMoneyShopInfo_reflection_;
  delete AckExchangeMoney::default_instance_;
  delete AckExchangeMoney_reflection_;
}

void protobuf_AddDesc_CharLogin_2fMoneyShop_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031CharLogin/MoneyShop.proto\022\rMoneyShopIn"
    "fo\"A\n\020AckMoneyShopInfo\022\013\n\003ret\030\001 \002(\005\022\017\n\007f"
    "reeNum\030\002 \001(\005\022\017\n\007costNum\030\003 \001(\005\"c\n\020AckExch"
    "angeMoney\022\013\n\003ret\030\001 \002(\005\022\r\n\005money\030\002 \001(\005\022\017\n"
    "\007freeNum\030\003 \001(\005\022\017\n\007costNum\030\004 \001(\005\022\021\n\tcritV"
    "alue\030\005 \001(\005", 210);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "CharLogin/MoneyShop.proto", &protobuf_RegisterTypes);
  AckMoneyShopInfo::default_instance_ = new AckMoneyShopInfo();
  AckExchangeMoney::default_instance_ = new AckExchangeMoney();
  AckMoneyShopInfo::default_instance_->InitAsDefaultInstance();
  AckExchangeMoney::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_CharLogin_2fMoneyShop_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_CharLogin_2fMoneyShop_2eproto {
  StaticDescriptorInitializer_CharLogin_2fMoneyShop_2eproto() {
    protobuf_AddDesc_CharLogin_2fMoneyShop_2eproto();
  }
} static_descriptor_initializer_CharLogin_2fMoneyShop_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int AckMoneyShopInfo::kRetFieldNumber;
const int AckMoneyShopInfo::kFreeNumFieldNumber;
const int AckMoneyShopInfo::kCostNumFieldNumber;
#endif  // !_MSC_VER

AckMoneyShopInfo::AckMoneyShopInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AckMoneyShopInfo::InitAsDefaultInstance() {
}

AckMoneyShopInfo::AckMoneyShopInfo(const AckMoneyShopInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AckMoneyShopInfo::SharedCtor() {
  _cached_size_ = 0;
  ret_ = 0;
  freenum_ = 0;
  costnum_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AckMoneyShopInfo::~AckMoneyShopInfo() {
  SharedDtor();
}

void AckMoneyShopInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AckMoneyShopInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AckMoneyShopInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AckMoneyShopInfo_descriptor_;
}

const AckMoneyShopInfo& AckMoneyShopInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_CharLogin_2fMoneyShop_2eproto();
  return *default_instance_;
}

AckMoneyShopInfo* AckMoneyShopInfo::default_instance_ = NULL;

AckMoneyShopInfo* AckMoneyShopInfo::New() const {
  return new AckMoneyShopInfo;
}

void AckMoneyShopInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ret_ = 0;
    freenum_ = 0;
    costnum_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AckMoneyShopInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ret_)));
          set_has_ret();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_freeNum;
        break;
      }

      // optional int32 freeNum = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_freeNum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &freenum_)));
          set_has_freenum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_costNum;
        break;
      }

      // optional int32 costNum = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_costNum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &costnum_)));
          set_has_costnum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void AckMoneyShopInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->ret(), output);
  }

  // optional int32 freeNum = 2;
  if (has_freenum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->freenum(), output);
  }

  // optional int32 costNum = 3;
  if (has_costnum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->costnum(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AckMoneyShopInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->ret(), target);
  }

  // optional int32 freeNum = 2;
  if (has_freenum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->freenum(), target);
  }

  // optional int32 costNum = 3;
  if (has_costnum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->costnum(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AckMoneyShopInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ret());
    }

    // optional int32 freeNum = 2;
    if (has_freenum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->freenum());
    }

    // optional int32 costNum = 3;
    if (has_costnum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->costnum());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AckMoneyShopInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AckMoneyShopInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AckMoneyShopInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AckMoneyShopInfo::MergeFrom(const AckMoneyShopInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
    if (from.has_freenum()) {
      set_freenum(from.freenum());
    }
    if (from.has_costnum()) {
      set_costnum(from.costnum());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AckMoneyShopInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AckMoneyShopInfo::CopyFrom(const AckMoneyShopInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AckMoneyShopInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void AckMoneyShopInfo::Swap(AckMoneyShopInfo* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(freenum_, other->freenum_);
    std::swap(costnum_, other->costnum_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AckMoneyShopInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AckMoneyShopInfo_descriptor_;
  metadata.reflection = AckMoneyShopInfo_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int AckExchangeMoney::kRetFieldNumber;
const int AckExchangeMoney::kMoneyFieldNumber;
const int AckExchangeMoney::kFreeNumFieldNumber;
const int AckExchangeMoney::kCostNumFieldNumber;
const int AckExchangeMoney::kCritValueFieldNumber;
#endif  // !_MSC_VER

AckExchangeMoney::AckExchangeMoney()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AckExchangeMoney::InitAsDefaultInstance() {
}

AckExchangeMoney::AckExchangeMoney(const AckExchangeMoney& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AckExchangeMoney::SharedCtor() {
  _cached_size_ = 0;
  ret_ = 0;
  money_ = 0;
  freenum_ = 0;
  costnum_ = 0;
  critvalue_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AckExchangeMoney::~AckExchangeMoney() {
  SharedDtor();
}

void AckExchangeMoney::SharedDtor() {
  if (this != default_instance_) {
  }
}

void AckExchangeMoney::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AckExchangeMoney::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AckExchangeMoney_descriptor_;
}

const AckExchangeMoney& AckExchangeMoney::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_CharLogin_2fMoneyShop_2eproto();
  return *default_instance_;
}

AckExchangeMoney* AckExchangeMoney::default_instance_ = NULL;

AckExchangeMoney* AckExchangeMoney::New() const {
  return new AckExchangeMoney;
}

void AckExchangeMoney::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ret_ = 0;
    money_ = 0;
    freenum_ = 0;
    costnum_ = 0;
    critvalue_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AckExchangeMoney::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ret_)));
          set_has_ret();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_money;
        break;
      }

      // optional int32 money = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_money:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &money_)));
          set_has_money();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_freeNum;
        break;
      }

      // optional int32 freeNum = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_freeNum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &freenum_)));
          set_has_freenum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_costNum;
        break;
      }

      // optional int32 costNum = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_costNum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &costnum_)));
          set_has_costnum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_critValue;
        break;
      }

      // optional int32 critValue = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_critValue:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &critvalue_)));
          set_has_critvalue();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void AckExchangeMoney::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->ret(), output);
  }

  // optional int32 money = 2;
  if (has_money()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->money(), output);
  }

  // optional int32 freeNum = 3;
  if (has_freenum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->freenum(), output);
  }

  // optional int32 costNum = 4;
  if (has_costnum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->costnum(), output);
  }

  // optional int32 critValue = 5;
  if (has_critvalue()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->critvalue(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AckExchangeMoney::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->ret(), target);
  }

  // optional int32 money = 2;
  if (has_money()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->money(), target);
  }

  // optional int32 freeNum = 3;
  if (has_freenum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->freenum(), target);
  }

  // optional int32 costNum = 4;
  if (has_costnum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->costnum(), target);
  }

  // optional int32 critValue = 5;
  if (has_critvalue()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->critvalue(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AckExchangeMoney::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ret());
    }

    // optional int32 money = 2;
    if (has_money()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->money());
    }

    // optional int32 freeNum = 3;
    if (has_freenum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->freenum());
    }

    // optional int32 costNum = 4;
    if (has_costnum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->costnum());
    }

    // optional int32 critValue = 5;
    if (has_critvalue()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->critvalue());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void AckExchangeMoney::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AckExchangeMoney* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AckExchangeMoney*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AckExchangeMoney::MergeFrom(const AckExchangeMoney& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
    if (from.has_money()) {
      set_money(from.money());
    }
    if (from.has_freenum()) {
      set_freenum(from.freenum());
    }
    if (from.has_costnum()) {
      set_costnum(from.costnum());
    }
    if (from.has_critvalue()) {
      set_critvalue(from.critvalue());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AckExchangeMoney::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AckExchangeMoney::CopyFrom(const AckExchangeMoney& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AckExchangeMoney::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void AckExchangeMoney::Swap(AckExchangeMoney* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(money_, other->money_);
    std::swap(freenum_, other->freenum_);
    std::swap(costnum_, other->costnum_);
    std::swap(critvalue_, other->critvalue_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AckExchangeMoney::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AckExchangeMoney_descriptor_;
  metadata.reflection = AckExchangeMoney_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MoneyShopInfo

// @@protoc_insertion_point(global_scope)
