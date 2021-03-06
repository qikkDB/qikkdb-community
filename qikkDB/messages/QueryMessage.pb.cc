// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages/QueryMessage.proto

#include "messages/QueryMessage.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace QikkDB {
namespace NetworkClient {
namespace Message {
class QueryMessageDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<QueryMessage> _instance;
} _QueryMessage_default_instance_;
}  // namespace Message
}  // namespace NetworkClient
}  // namespace QikkDB
static void InitDefaultsscc_info_QueryMessage_messages_2fQueryMessage_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::QikkDB::NetworkClient::Message::_QueryMessage_default_instance_;
    new (ptr) ::QikkDB::NetworkClient::Message::QueryMessage();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::QikkDB::NetworkClient::Message::QueryMessage::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_QueryMessage_messages_2fQueryMessage_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_QueryMessage_messages_2fQueryMessage_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_messages_2fQueryMessage_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_messages_2fQueryMessage_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_messages_2fQueryMessage_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_messages_2fQueryMessage_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::QikkDB::NetworkClient::Message::QueryMessage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::QikkDB::NetworkClient::Message::QueryMessage, query_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::QikkDB::NetworkClient::Message::QueryMessage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::QikkDB::NetworkClient::Message::_QueryMessage_default_instance_),
};

const char descriptor_table_protodef_messages_2fQueryMessage_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\033messages/QueryMessage.proto\022\034QikkDB.Ne"
  "tworkClient.Message\"\035\n\014QueryMessage\022\r\n\005Q"
  "uery\030\001 \001(\tb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_messages_2fQueryMessage_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_messages_2fQueryMessage_2eproto_sccs[1] = {
  &scc_info_QueryMessage_messages_2fQueryMessage_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_messages_2fQueryMessage_2eproto_once;
static bool descriptor_table_messages_2fQueryMessage_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_messages_2fQueryMessage_2eproto = {
  &descriptor_table_messages_2fQueryMessage_2eproto_initialized, descriptor_table_protodef_messages_2fQueryMessage_2eproto, "messages/QueryMessage.proto", 98,
  &descriptor_table_messages_2fQueryMessage_2eproto_once, descriptor_table_messages_2fQueryMessage_2eproto_sccs, descriptor_table_messages_2fQueryMessage_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_messages_2fQueryMessage_2eproto::offsets,
  file_level_metadata_messages_2fQueryMessage_2eproto, 1, file_level_enum_descriptors_messages_2fQueryMessage_2eproto, file_level_service_descriptors_messages_2fQueryMessage_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_messages_2fQueryMessage_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_messages_2fQueryMessage_2eproto), true);
namespace QikkDB {
namespace NetworkClient {
namespace Message {

// ===================================================================

void QueryMessage::InitAsDefaultInstance() {
}
class QueryMessage::_Internal {
 public:
};

QueryMessage::QueryMessage()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:QikkDB.NetworkClient.Message.QueryMessage)
}
QueryMessage::QueryMessage(const QueryMessage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  query_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_query().empty()) {
    query_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.query_);
  }
  // @@protoc_insertion_point(copy_constructor:QikkDB.NetworkClient.Message.QueryMessage)
}

void QueryMessage::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_QueryMessage_messages_2fQueryMessage_2eproto.base);
  query_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

QueryMessage::~QueryMessage() {
  // @@protoc_insertion_point(destructor:QikkDB.NetworkClient.Message.QueryMessage)
  SharedDtor();
}

void QueryMessage::SharedDtor() {
  query_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void QueryMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const QueryMessage& QueryMessage::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_QueryMessage_messages_2fQueryMessage_2eproto.base);
  return *internal_default_instance();
}


void QueryMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:QikkDB.NetworkClient.Message.QueryMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  query_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

const char* QueryMessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string Query = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(_internal_mutable_query(), ptr, ctx, "QikkDB.NetworkClient.Message.QueryMessage.Query");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* QueryMessage::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:QikkDB.NetworkClient.Message.QueryMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string Query = 1;
  if (this->query().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_query().data(), static_cast<int>(this->_internal_query().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "QikkDB.NetworkClient.Message.QueryMessage.Query");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_query(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:QikkDB.NetworkClient.Message.QueryMessage)
  return target;
}

size_t QueryMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:QikkDB.NetworkClient.Message.QueryMessage)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string Query = 1;
  if (this->query().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_query());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void QueryMessage::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:QikkDB.NetworkClient.Message.QueryMessage)
  GOOGLE_DCHECK_NE(&from, this);
  const QueryMessage* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<QueryMessage>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:QikkDB.NetworkClient.Message.QueryMessage)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:QikkDB.NetworkClient.Message.QueryMessage)
    MergeFrom(*source);
  }
}

void QueryMessage::MergeFrom(const QueryMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:QikkDB.NetworkClient.Message.QueryMessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.query().size() > 0) {

    query_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.query_);
  }
}

void QueryMessage::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:QikkDB.NetworkClient.Message.QueryMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void QueryMessage::CopyFrom(const QueryMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:QikkDB.NetworkClient.Message.QueryMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool QueryMessage::IsInitialized() const {
  return true;
}

void QueryMessage::InternalSwap(QueryMessage* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  query_.Swap(&other->query_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
}

::PROTOBUF_NAMESPACE_ID::Metadata QueryMessage::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Message
}  // namespace NetworkClient
}  // namespace QikkDB
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::QikkDB::NetworkClient::Message::QueryMessage* Arena::CreateMaybeMessage< ::QikkDB::NetworkClient::Message::QueryMessage >(Arena* arena) {
  return Arena::CreateInternal< ::QikkDB::NetworkClient::Message::QueryMessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
