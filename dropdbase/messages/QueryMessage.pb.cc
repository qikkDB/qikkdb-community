// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: QueryMessage.proto

#include "QueryMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace ColmnarDB {
namespace NetworkClient {
namespace Message {
class QueryMessageDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<QueryMessage>
      _instance;
} _QueryMessage_default_instance_;
}  // namespace Message
}  // namespace NetworkClient
}  // namespace ColmnarDB
namespace protobuf_QueryMessage_2eproto {
static void InitDefaultsQueryMessage() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ColmnarDB::NetworkClient::Message::_QueryMessage_default_instance_;
    new (ptr) ::ColmnarDB::NetworkClient::Message::QueryMessage();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ColmnarDB::NetworkClient::Message::QueryMessage::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_QueryMessage =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsQueryMessage}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_QueryMessage.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::ColmnarDB::NetworkClient::Message::QueryMessage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::ColmnarDB::NetworkClient::Message::QueryMessage, query_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::ColmnarDB::NetworkClient::Message::QueryMessage)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::ColmnarDB::NetworkClient::Message::_QueryMessage_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "QueryMessage.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\022QueryMessage.proto\022\037ColmnarDB.NetworkC"
      "lient.Message\"\035\n\014QueryMessage\022\r\n\005Query\030\001"
      " \001(\tb\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 92);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "QueryMessage.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_QueryMessage_2eproto
namespace ColmnarDB {
namespace NetworkClient {
namespace Message {

// ===================================================================

void QueryMessage::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int QueryMessage::kQueryFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

QueryMessage::QueryMessage()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_QueryMessage_2eproto::scc_info_QueryMessage.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:ColmnarDB.NetworkClient.Message.QueryMessage)
}
QueryMessage::QueryMessage(const QueryMessage& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  query_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.query().size() > 0) {
    query_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.query_);
  }
  // @@protoc_insertion_point(copy_constructor:ColmnarDB.NetworkClient.Message.QueryMessage)
}

void QueryMessage::SharedCtor() {
  query_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

QueryMessage::~QueryMessage() {
  // @@protoc_insertion_point(destructor:ColmnarDB.NetworkClient.Message.QueryMessage)
  SharedDtor();
}

void QueryMessage::SharedDtor() {
  query_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void QueryMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* QueryMessage::descriptor() {
  ::protobuf_QueryMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_QueryMessage_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const QueryMessage& QueryMessage::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_QueryMessage_2eproto::scc_info_QueryMessage.base);
  return *internal_default_instance();
}


void QueryMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  query_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

bool QueryMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string Query = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_query()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->query().data(), static_cast<int>(this->query().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "ColmnarDB.NetworkClient.Message.QueryMessage.Query"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:ColmnarDB.NetworkClient.Message.QueryMessage)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:ColmnarDB.NetworkClient.Message.QueryMessage)
  return false;
#undef DO_
}

void QueryMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string Query = 1;
  if (this->query().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->query().data(), static_cast<int>(this->query().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "ColmnarDB.NetworkClient.Message.QueryMessage.Query");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->query(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:ColmnarDB.NetworkClient.Message.QueryMessage)
}

::google::protobuf::uint8* QueryMessage::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string Query = 1;
  if (this->query().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->query().data(), static_cast<int>(this->query().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "ColmnarDB.NetworkClient.Message.QueryMessage.Query");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->query(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ColmnarDB.NetworkClient.Message.QueryMessage)
  return target;
}

size_t QueryMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string Query = 1;
  if (this->query().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->query());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void QueryMessage::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  GOOGLE_DCHECK_NE(&from, this);
  const QueryMessage* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const QueryMessage>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ColmnarDB.NetworkClient.Message.QueryMessage)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ColmnarDB.NetworkClient.Message.QueryMessage)
    MergeFrom(*source);
  }
}

void QueryMessage::MergeFrom(const QueryMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.query().size() > 0) {

    query_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.query_);
  }
}

void QueryMessage::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void QueryMessage::CopyFrom(const QueryMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ColmnarDB.NetworkClient.Message.QueryMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool QueryMessage::IsInitialized() const {
  return true;
}

void QueryMessage::Swap(QueryMessage* other) {
  if (other == this) return;
  InternalSwap(other);
}
void QueryMessage::InternalSwap(QueryMessage* other) {
  using std::swap;
  query_.Swap(&other->query_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata QueryMessage::GetMetadata() const {
  protobuf_QueryMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_QueryMessage_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Message
}  // namespace NetworkClient
}  // namespace ColmnarDB
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::ColmnarDB::NetworkClient::Message::QueryMessage* Arena::CreateMaybeMessage< ::ColmnarDB::NetworkClient::Message::QueryMessage >(Arena* arena) {
  return Arena::CreateInternal< ::ColmnarDB::NetworkClient::Message::QueryMessage >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
