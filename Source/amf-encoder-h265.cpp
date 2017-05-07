/*
MIT License

Copyright (c) 2016-2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "amf-encoder-h265.h"
#include "utility.h"
#include <inttypes.h>

#define QUICK_THROW_ERROR(format, ...) {\
	QUICK_FORMAT_MESSAGE(errMsg, __FUNCTION_NAME__ format, \
		m_UniqueId,  ##__VA_ARGS__, \
		m_AMF->GetTrace()->GetResultText(res), res); \
	throw std::exception(errMsg.c_str()); \
}
#define PREFIX "[H265]<Id: %lld> "

using namespace Plugin;
using namespace Plugin::AMD;
using namespace Utility;

Plugin::AMD::EncoderH265::EncoderH265(
	std::shared_ptr<API::IAPI> videoAPI, API::Adapter videoAdapter,
	bool useOpenCLSubmission, bool useOpenCLConversion,
	ColorFormat colorFormat, ColorSpace colorSpace, bool fullRangeColor,
	bool useAsyncQueue, size_t asyncQueueSize)
	: Encoder(Codec::HEVC,
		videoAPI, videoAdapter,
		useOpenCLSubmission, useOpenCLConversion,
		colorFormat, colorSpace, fullRangeColor,
		useAsyncQueue, asyncQueueSize) {
	AMFTRACECALL;
}


Plugin::AMD::EncoderH265::~EncoderH265() {
	AMFTRACECALL;
}

// Initialization
std::vector<Usage> Plugin::AMD::EncoderH265::CapsUsage() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_USAGE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<Usage> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::UsageFromAMFH265((AMF_VIDEO_ENCODER_HEVC_USAGE_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetUsage(Usage v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_USAGE, Utility::UsageToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::UsageToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::Usage Plugin::AMD::EncoderH265::GetUsage() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_USAGE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::UsageFromAMFH265((AMF_VIDEO_ENCODER_HEVC_USAGE_ENUM)e);
}

// Static
std::vector<QualityPreset> Plugin::AMD::EncoderH265::CapsQualityPreset() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<QualityPreset> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::QualityPresetFromAMFH265((AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetQualityPreset(QualityPreset v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET, Utility::QualityPresetToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::QualityPresetToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::QualityPreset Plugin::AMD::EncoderH265::GetQualityPreset() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::QualityPresetFromAMFH265((AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_ENUM)e);
}

std::pair<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t>> Plugin::AMD::EncoderH265::CapsResolution() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_FRAMESIZE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair(
		std::make_pair(var->minValue.sizeValue.width, var->maxValue.sizeValue.width),
		std::make_pair(var->minValue.sizeValue.height, var->maxValue.sizeValue.height)
	);
}

void Plugin::AMD::EncoderH265::SetResolution(std::pair<uint32_t, uint32_t> v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_FRAMESIZE, ::AMFConstructSize(v.first, v.second));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ldx%ld, error %ls (code %d)",
			m_UniqueId, v.first, v.second, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_Resolution.first = v.first;
	m_Resolution.second = v.second;
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::GetResolution() {
	AMFTRACECALL;

	AMFSize e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_FRAMESIZE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_Resolution.first = e.width;
	m_Resolution.second = e.height;
	return std::make_pair(e.width, e.height);
}

void Plugin::AMD::EncoderH265::SetAspectRatio(std::pair<uint32_t, uint32_t> v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_ASPECT_RATIO, ::AMFConstructRatio(v.first, v.second));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld:%ld, error %ls (code %d)",
			m_UniqueId, v.first, v.second, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::GetAspectRatio() {
	AMFTRACECALL;

	AMFRatio e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_ASPECT_RATIO, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return std::make_pair(e.num, e.den);
}

void Plugin::AMD::EncoderH265::SetFrameRate(std::pair<uint32_t, uint32_t> v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_FRAMERATE, ::AMFConstructRate(v.first, v.second));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld/%ld, error %ls (code %d)",
			m_UniqueId, v.first, v.second, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_FrameRate = std::make_pair(v.first, v.second);
	UpdateFrameRateValues();
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::GetFrameRate() {
	AMFTRACECALL;

	AMFRate e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_FRAMERATE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> Unable to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_FrameRate = std::make_pair(e.num, e.den);
	UpdateFrameRateValues();
	return m_FrameRate;
}

std::vector<Profile> Plugin::AMD::EncoderH265::CapsProfile() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_PROFILE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<Profile> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::ProfileFromAMFH265((AMF_VIDEO_ENCODER_HEVC_PROFILE_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetProfile(Profile v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_PROFILE, Utility::ProfileToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::ProfileToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::Profile Plugin::AMD::EncoderH265::GetProfile() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_PROFILE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::ProfileFromAMFH265((AMF_VIDEO_ENCODER_HEVC_PROFILE_ENUM)e);
}

std::vector<ProfileLevel> Plugin::AMD::EncoderH265::CapsProfileLevel() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_PROFILE_LEVEL, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<ProfileLevel> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back((ProfileLevel)(enm->value / 3));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetProfileLevel(ProfileLevel v) {
	AMFTRACECALL;;

	if (v == ProfileLevel::Automatic)
		v = Utility::H265ProfileLevel(m_Resolution, m_FrameRate);

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_PROFILE_LEVEL, ((int64_t)v) * 3);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lld, error %ls (code %d)",
			m_UniqueId, (int64_t)v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::ProfileLevel Plugin::AMD::EncoderH265::GetProfileLevel() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_PROFILE_LEVEL, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (ProfileLevel)(e / 3);
}

std::vector<H265::Tier> Plugin::AMD::EncoderH265::CapsTier() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_TIER, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<H265::Tier> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::TierFromAMFH265((AMF_VIDEO_ENCODER_HEVC_TIER_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetTier(H265::Tier v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_TIER, Utility::TierToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::TierToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::H265::Tier Plugin::AMD::EncoderH265::GetTier() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_TIER, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (H265::Tier)e;
}

std::pair<uint64_t, uint64_t> Plugin::AMD::EncoderH265::CapsMaximumReferenceFrames() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_MAX_NUM_REFRAMES, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair(var->minValue.int64Value, var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetMaximumReferenceFrames(uint64_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_NUM_REFRAMES, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint64_t Plugin::AMD::EncoderH265::GetMaximumReferenceFrames() {
	AMFTRACECALL;

	uint64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_NUM_REFRAMES, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, __FUNCTION_NAME__ "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

std::vector<CodingType> Plugin::AMD::EncoderH265::CapsCodingType() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_CABAC_ENABLE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<CodingType> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::CodingTypeFromAMFH265((AMF_VIDEO_ENCODER_CODING_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetCodingType(CodingType v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_CABAC_ENABLE, Utility::CodingTypeToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::CodingTypeToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::CodingType Plugin::AMD::EncoderH265::GetCodingType() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_CABAC_ENABLE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> Unable to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::CodingTypeFromAMFH265((AMF_VIDEO_ENCODER_CODING_ENUM)e);
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::CapsMaximumLongTermReferenceFrames() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_MAX_LTR_FRAMES, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair((uint32_t)var->minValue.int64Value, (uint32_t)var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetMaximumLongTermReferenceFrames(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_LTR_FRAMES, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetMaximumLongTermReferenceFrames() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_LTR_FRAMES, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

/// Rate Control
std::vector<RateControlMethod> Plugin::AMD::EncoderH265::CapsRateControlMethod() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<RateControlMethod> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::RateControlMethodFromAMFH265((AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_ENUM)enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetRateControlMethod(RateControlMethod v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD, Utility::RateControlMethodToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, Utility::RateControlMethodToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::RateControlMethod Plugin::AMD::EncoderH265::GetRateControlMethod() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::RateControlMethodFromAMFH265((AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_ENUM)e);
}

std::vector<PrePassMode> Plugin::AMD::EncoderH265::CapsPrePassMode() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_PREANALYSIS_ENABLE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	if (var->type == amf::AMF_VARIANT_BOOL) {
		return std::vector<PrePassMode>({ PrePassMode::Disabled, PrePassMode::Enabled });
	} else {
		std::vector<PrePassMode> ret;
		for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
			PLOG_ERROR("Unknown Pre-Pass Mode: %ls %lld", enm->name, enm->value);
			//ret.push_back(Utility::PrePassModeFromAMFH265((AMF_VIDEO_ENCODER_PREENCODE_MODE_ENUM)enm->value));
		}
		return ret;
	}
}

void Plugin::AMD::EncoderH265::SetPrePassMode(PrePassMode v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_PREANALYSIS_ENABLE, v != PrePassMode::Disabled);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, (v != PrePassMode::Disabled) ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::PrePassMode Plugin::AMD::EncoderH265::GetPrePassMode() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_PREANALYSIS_ENABLE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> Unable to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	if (e) {
		return PrePassMode::Enabled;
	} else {
		return PrePassMode::Disabled;
	}
}

void Plugin::AMD::EncoderH265::SetVarianceBasedAdaptiveQuantizationEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_ENABLE_VBAQ, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsVarianceBasedAdaptiveQuantizationEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_ENABLE_VBAQ, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

/// VBV Buffer
std::pair<uint64_t, uint64_t> Plugin::AMD::EncoderH265::CapsVBVBufferSize() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_VBV_BUFFER_SIZE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair(var->minValue.int64Value, var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetVBVBufferSize(uint64_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_VBV_BUFFER_SIZE, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint64_t Plugin::AMD::EncoderH265::GetVBVBufferSize() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_VBV_BUFFER_SIZE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetVBVBufferInitialFullness(double v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_INITIAL_VBV_BUFFER_FULLNESS, (int64_t)(v * 64));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lf (%d), error %ls (code %d)",
			m_UniqueId, v, (uint8_t)(v * 64), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

float Plugin::AMD::EncoderH265::GetInitialVBVBufferFullness() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_INITIAL_VBV_BUFFER_FULLNESS, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (e / 64.0f);
}

/// Picture Control
std::vector<H265::GOPType> Plugin::AMD::EncoderH265::CapsGOPType() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(L"GOPType", &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	std::vector<H265::GOPType> ret;
	for (const amf::AMFEnumDescriptionEntry* enm = var->pEnumDescription; enm->name != nullptr; enm++) {
		ret.push_back(Utility::GOPTypeFromAMFH265(enm->value));
	}
	return ret;
}

void Plugin::AMD::EncoderH265::SetGOPType(H265::GOPType v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"GOPType", Utility::GOPTypeToAMFH265(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %s, error %ls (code %d)",
			m_UniqueId, Utility::GOPTypeToString(v), m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::H265::GOPType Plugin::AMD::EncoderH265::GetGOPType() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"GOPType", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return Utility::GOPTypeFromAMFH265(e);
}

void Plugin::AMD::EncoderH265::SetGOPSize(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_GOP_SIZE, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetGOPSize() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_GOP_SIZE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetGOPSizeMin(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"GOPSizeMin", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetGOPSizeMin() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"GOPSizeMin", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetGOPSizeMax(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"GOPSizeMax", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetGOPSizeMax() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"GOPSizeMax", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetGOPAlignmentEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"EnableGOPAlignment", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsGOPAlignmentEnabled() {
	AMFTRACECALL;

	bool e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"EnableGOPAlignment", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetIDRPeriod(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_NUM_GOPS_PER_IDR, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_PeriodIDR = v;
}

uint32_t Plugin::AMD::EncoderH265::GetIDRPeriod() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_NUM_GOPS_PER_IDR, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	m_PeriodIDR = (uint32_t)e;
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetHeaderInsertionMode(H265::HeaderInsertionMode v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE, static_cast<AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_ENUM>(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::H265::HeaderInsertionMode Plugin::AMD::EncoderH265::GetHeaderInsertionMode() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return static_cast<H265::HeaderInsertionMode>(e);
}

void Plugin::AMD::EncoderH265::SetDeblockingFilterEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_DE_BLOCKING_FILTER_DISABLE, !v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsDeblockingFilterEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(L"HevcDeBlockingFilter", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

/// Motion Estimation
void Plugin::AMD::EncoderH265::SetMotionEstimationQuarterPixelEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MOTION_QUARTERPIXEL, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsMotionEstimationQuarterPixelEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MOTION_QUARTERPIXEL, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetMotionEstimationHalfPixelEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MOTION_HALF_PIXEL, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsMotionEstimationHalfPixelEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MOTION_HALF_PIXEL, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

// Dynamic
void Plugin::AMD::EncoderH265::SetFrameSkippingEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_SKIP_FRAME_ENABLE, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsFrameSkippingEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_SKIP_FRAME_ENABLE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetEnforceHRDEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_ENFORCE_HRD, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsEnforceHRDEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_ENFORCE_HRD, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetFillerDataEnabled(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_FILLER_DATA_ENABLE, v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::IsFillerDataEnabled() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_FILLER_DATA_ENABLE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetIFrameQPMinimum(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MIN_QP_I, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetIFrameQPMinimum() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MIN_QP_I, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetIFrameQPMaximum(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_QP_I, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetIFrameQPMaximum() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_QP_I, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetPFrameQPMinimum(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MIN_QP_P, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetPFrameQPMinimum() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MIN_QP_P, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetPFrameQPMaximum(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_QP_P, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetPFrameQPMaximum() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_QP_P, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

std::pair<uint64_t, uint64_t> Plugin::AMD::EncoderH265::CapsTargetBitrate() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_TARGET_BITRATE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair(var->minValue.int64Value, var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetTargetBitrate(uint64_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_TARGET_BITRATE, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint64_t Plugin::AMD::EncoderH265::GetTargetBitrate() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_TARGET_BITRATE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

std::pair<uint64_t, uint64_t> Plugin::AMD::EncoderH265::CapsPeakBitrate() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(AMF_VIDEO_ENCODER_HEVC_PEAK_BITRATE, &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair(var->minValue.int64Value, var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetPeakBitrate(uint64_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_PEAK_BITRATE, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %lld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint64_t Plugin::AMD::EncoderH265::GetPeakBitrate() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_PEAK_BITRATE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetIFrameQP(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_QP_I, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetIFrameQP() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_QP_I, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetPFrameQP(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_QP_P, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %d, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetPFrameQP() {
	AMFTRACECALL;

	int64_t e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_QP_P, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetMaximumAccessUnitSize(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_AU_SIZE, (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetMaximumAccessUnitSize() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_MAX_AU_SIZE, &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

// Intra-Refresh
void Plugin::AMD::EncoderH265::SetIntraRefreshMode(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"IntraRefreshMode", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetIntraRefreshMode() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"IntraRefreshMode", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetIntraRefreshFrameNum(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"HevcIntraRefreshFrameNum", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetIntraRefreshFrameNum() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"HevcIntraRefreshFrameNum", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

// Properties - Slicing
std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::CapsSlicesPerFrame() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(L"HevcSlicesPerFrame", &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair((uint32_t)var->minValue.int64Value, (uint32_t)var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetSlicesPerFrame(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"HevcSlicesPerFrame", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetSlicesPerFrame() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"HevcSlicesPerFrame", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetSliceControlMode(SliceControlMode v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"SliceControlMode", static_cast<int64_t>(v));
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

Plugin::AMD::SliceControlMode Plugin::AMD::EncoderH265::GetSliceControlMode() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"SliceControlMode", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return static_cast<SliceControlMode>(e);
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::CapsSliceControlSize() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(L"SliceControlSize", &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair((uint32_t)var->minValue.int64Value, (uint32_t)var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetSliceControlSize(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"SliceControlSize", (int64_t)v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetSliceControlSize() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"SliceControlSize", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

// Experimental
void Plugin::AMD::EncoderH265::SetQPCBOffset(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"QPCBOFFSET", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetQPCBOffset() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"QPCBOFFSET", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

void Plugin::AMD::EncoderH265::SetQPCROffset(uint8_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"QPCROFFSET", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint8_t Plugin::AMD::EncoderH265::GetQPCROffset() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"QPCROFFSET", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint8_t)e;
}

std::pair<uint32_t, uint32_t> Plugin::AMD::EncoderH265::CapsInputQueueSize() {
	AMFTRACECALL;

	const amf::AMFPropertyInfo* var;
	AMF_RESULT res = m_AMFEncoder->GetPropertyInfo(L"HevcInputQueueSize", &var);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Querying capabilities failed, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}

	return std::make_pair((uint32_t)var->minValue.int64Value, (uint32_t)var->maxValue.int64Value);
}

void Plugin::AMD::EncoderH265::SetInputQueueSize(uint32_t v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"HevcInputQueueSize", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %ld, error %ls (code %d)",
			m_UniqueId, v, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

uint32_t Plugin::AMD::EncoderH265::GetInputQueueSize() {
	AMFTRACECALL;

	int64_t e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"HevcInputQueueSize", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return (uint32_t)e;
}

void Plugin::AMD::EncoderH265::SetLowLatencyInternal(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"LowLatencyInternal", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::GetLowLatencyInternal() {
	AMFTRACECALL;

	bool e;

	AMF_RESULT res = m_AMFEncoder->GetProperty(L"LowLatencyInternal", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

void Plugin::AMD::EncoderH265::SetCommonLowLatencyInternal(bool v) {
	AMFTRACECALL;

	AMF_RESULT res = m_AMFEncoder->SetProperty(L"CommonLowLatencyInternal", v);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to set mode to %s, error %ls (code %d)",
			m_UniqueId, v ? "Enabled" : "Disabled", m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
}

bool Plugin::AMD::EncoderH265::GetCommonLowLatencyInternal() {
	AMFTRACECALL;

	bool e;
	AMF_RESULT res = m_AMFEncoder->GetProperty(L"CommonLowLatencyInternal", &e);
	if (res != AMF_OK) {
		QUICK_FORMAT_MESSAGE(errMsg, "<Id: %lld> <" __FUNCTION_NAME__ "> Failed to retrieve value, error %ls (code %d)",
			m_UniqueId, m_AMF->GetTrace()->GetResultText(res), res);
		throw std::exception(errMsg.c_str());
	}
	return e;
}

// Internal
void Plugin::AMD::EncoderH265::PacketPriorityAndKeyframe(amf::AMFDataPtr& pData, struct encoder_packet* packet) {
	AMFTRACECALL;

	uint64_t pktType;
	pData->GetProperty(AMF_VIDEO_ENCODER_HEVC_OUTPUT_DATA_TYPE, &pktType);
	switch ((AMF_VIDEO_ENCODER_HEVC_OUTPUT_DATA_TYPE_ENUM)pktType) {
		case AMF_VIDEO_ENCODER_HEVC_OUTPUT_DATA_TYPE_I:
			packet->keyframe = true;
			packet->priority = 1;
			break;
		case AMF_VIDEO_ENCODER_HEVC_OUTPUT_DATA_TYPE_P:
			packet->priority = 0;
			break;
	}
}

AMF_RESULT Plugin::AMD::EncoderH265::GetExtraDataInternal(amf::AMFVariant* p) {
	AMFTRACECALL;

	return m_AMFEncoder->GetProperty(AMF_VIDEO_ENCODER_HEVC_EXTRADATA, p);
}

std::string Plugin::AMD::EncoderH265::HandleTypeOverride(amf::AMFSurfacePtr & d, uint64_t index) {
	AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_ENUM type = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_NONE;

	if ((m_PeriodPFrame > 0) && ((index % m_PeriodPFrame) == 0)) {
		type = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_P;
	}
	if ((m_PeriodIFrame > 0) && ((index % m_PeriodIFrame) == 0)) {
		type = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_I;
	}
	uint64_t realIPeriod = m_PeriodIDR * GetGOPSize();
	if ((type != AMF_VIDEO_ENCODER_PICTURE_TYPE_NONE) && (realIPeriod > 0) && ((index % realIPeriod) == 0)) {
		type = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_IDR;
	}
	if (m_FrameSkipPeriod > 0) {
		bool shouldSkip = m_FrameSkipKeepOnlyNth
			? (index % m_FrameSkipPeriod) != 0
			: (index % m_FrameSkipPeriod) == 0;

		if (shouldSkip) {
			if ((m_FrameSkipType <= AMF_VIDEO_ENCODER_PICTURE_TYPE_SKIP) || (type < m_FrameSkipType))
				m_FrameSkipType = type;
			type = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_SKIP;
		} else if (m_FrameSkipType != AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_NONE) {
			type = m_FrameSkipType; // Hopefully fixes the crash.
			m_FrameSkipType = AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_NONE;
		}
	}
	d->SetProperty(AMF_VIDEO_ENCODER_FORCE_PICTURE_TYPE, type);

	switch (type) {
		case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_NONE:
			return "Automatic";
			break;
		case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_SKIP:
			return "Skip";
			break;
		case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_IDR:
			return "IDR";
			break;
		case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_I:
			return "I";
			break;
		case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_P:
			return "P";
			break;
		//case AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_B:
		//	return "B";
		//	break;
	}
	return "Unknown";
}

void Plugin::AMD::EncoderH265::LogProperties() {
	AMFTRACECALL;

	PLOG_INFO(PREFIX "Encoder Parameters:",
		m_UniqueId);
	#pragma region Backend
	PLOG_INFO(PREFIX "  Backend:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Video API: %s",
		m_UniqueId,
		m_API->GetName().c_str());
	PLOG_INFO(PREFIX "    Video Adapter: %s",
		m_UniqueId,
		m_APIAdapter.Name.c_str());
	PLOG_INFO(PREFIX "    OpenCL: %s",
		m_UniqueId,
		m_OpenCL ? "Supported" : "Not Supported");
	PLOG_INFO(PREFIX "      Transfer: %s",
		m_UniqueId,
		m_OpenCLSubmission ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "      Conversion: %s",
		m_UniqueId,
		m_OpenCLConversion ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "    Async Queue: %s",
		m_UniqueId,
		m_AsyncQueue ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "      Size: %" PRIu32,
		m_UniqueId,
		(uint32_t)m_AsyncQueueSize);
	#pragma endregion Backend
	#pragma region Frame
	PLOG_INFO(PREFIX "  Frame:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Format: %s %s %s",
		m_UniqueId,
		Utility::ColorFormatToString(m_ColorFormat),
		Utility::ColorSpaceToString(m_ColorSpace),
		m_FullColorRange ? "Full" : "Partial");
	PLOG_INFO(PREFIX "    Resolution: %" PRIu32 "x%" PRIu32,
		m_UniqueId,
		m_Resolution.first,
		m_Resolution.second);
	PLOG_INFO(PREFIX "    Frame Rate: %" PRIu32 "/%" PRIu32,
		m_UniqueId,
		m_FrameRate.first,
		m_FrameRate.second);
	auto aspectRatio = GetAspectRatio();
	PLOG_INFO(PREFIX "    Aspect Ratio: %" PRIu32 ":%" PRIu32,
		m_UniqueId,
		aspectRatio.first,
		aspectRatio.second);
	#pragma endregion Frame
	#pragma region Static
	PLOG_INFO(PREFIX "  Static:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Usage: %s",
		m_UniqueId,
		Utility::UsageToString(GetUsage()));
	PLOG_INFO(PREFIX "    Quality Preset: %s",
		m_UniqueId,
		Utility::QualityPresetToString(GetQualityPreset()));
	auto profileLevel = static_cast<uint16_t>(GetProfileLevel());
	PLOG_INFO(PREFIX "    Profile: %s %" PRIu16 ".%" PRIu16,
		m_UniqueId,
		Utility::ProfileToString(GetProfile()),
		profileLevel / 10,
		profileLevel % 10);
	PLOG_INFO(PREFIX "    Tier: %s",
		m_UniqueId,
		Utility::TierToString(GetTier()));
	PLOG_INFO(PREFIX "    Coding Type: %s",
		m_UniqueId,
		Utility::CodingTypeToString(GetCodingType()));
	PLOG_INFO(PREFIX "    Max. Reference Frames: %" PRIu16,
		m_UniqueId,
		(uint16_t)GetMaximumReferenceFrames());
	PLOG_INFO(PREFIX "    Max. Long-Term Reference Frames: %" PRIu16,
		m_UniqueId,
		(uint16_t)GetMaximumLongTermReferenceFrames());
	#pragma endregion Static
	#pragma region Rate Control
	PLOG_INFO(PREFIX "  Rate Control:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Method: %s",
		m_UniqueId,
		Utility::RateControlMethodToString(GetRateControlMethod()));
	PLOG_INFO(PREFIX "    Pre-Pass Mode: %s",
		m_UniqueId,
		Utility::PrePassModeToString(GetPrePassMode()));
	#pragma region QP
	PLOG_INFO(PREFIX "    QP:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      Ranges:",
		m_UniqueId);
	PLOG_INFO(PREFIX "        I-Frame: %" PRIu8 " - %" PRIu8,
		m_UniqueId,
		GetIFrameQPMinimum(),
		GetIFrameQPMaximum());
	PLOG_INFO(PREFIX "        P-Frame: %" PRIu8 " - %" PRIu8,
		m_UniqueId,
		GetPFrameQPMinimum(),
		GetPFrameQPMaximum());
	PLOG_INFO(PREFIX "      Fixed:",
		m_UniqueId);
	PLOG_INFO(PREFIX "        I-Frame: %" PRIu8,
		m_UniqueId,
		GetIFrameQP());
	PLOG_INFO(PREFIX "        P-Frame: %" PRIu8,
		m_UniqueId,
		GetPFrameQP());
	#pragma endregion QP
	#pragma region Bitrate
	PLOG_INFO(PREFIX "    Bitrate:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      Target: %" PRIu64 " bit/s",
		m_UniqueId,
		GetTargetBitrate());
	PLOG_INFO(PREFIX "      Peak: %" PRIu64 " bit/s",
		m_UniqueId,
		GetPeakBitrate());
	#pragma endregion Bitrate
	#pragma region Flags
	PLOG_INFO(PREFIX "    Flags:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      Filler Data: %s",
		m_UniqueId,
		IsFillerDataEnabled() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "      Frame Skipping: %s",
		m_UniqueId,
		IsFrameSkippingEnabled() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "        Period: %" PRIu32 " Frames",
		m_UniqueId,
		GetFrameSkippingPeriod());
	PLOG_INFO(PREFIX "        Behaviour: %s",
		m_UniqueId,
		GetFrameSkippingBehaviour() ? "Keep every Nth frame" : "Skip every Nth frame");
	PLOG_INFO(PREFIX "      Variance Based Adaptive Quantization: %s",
		m_UniqueId,
		IsVarianceBasedAdaptiveQuantizationEnabled() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "      Enforce Hypothetical Reference Decoder: %s",
		m_UniqueId,
		IsEnforceHRDEnabled() ? "Enabled" : "Disabled");
	#pragma endregion Flags
	#pragma region Video Buffering Verifier
	PLOG_INFO(PREFIX "    Video Buffering Verfier:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      Buffer Size: %" PRIu64 " bits",
		m_UniqueId,
		GetVBVBufferSize());
	PLOG_INFO(PREFIX "      Initial Fullness: %" PRIu64 " %%",
		m_UniqueId,
		(uint64_t)round(GetInitialVBVBufferFullness() * 100.0));
	#pragma endregion Video Buffering Verifier
	PLOG_INFO(PREFIX "    Max. Access Unit Size: %" PRIu32,
		m_UniqueId,
		GetMaximumAccessUnitSize());
	#pragma endregion Rate Control

	#pragma region Picture Control
	PLOG_INFO(PREFIX "  Picture Control:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Period:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      IDR: %" PRIu32 " GOPs",
		m_UniqueId,
		GetIDRPeriod());
	PLOG_INFO(PREFIX "      I: %" PRIu32 " Frames",
		m_UniqueId,
		GetIFramePeriod());
	PLOG_INFO(PREFIX "      P: %" PRIu32 " Frames",
		m_UniqueId,
		GetPFramePeriod());
	PLOG_INFO(PREFIX "      B: %" PRIu32 " Frames",
		m_UniqueId,
		GetBFramePeriod());
	PLOG_INFO(PREFIX "    GOP:",
		m_UniqueId);
	PLOG_INFO(PREFIX "      Type: %s",
		m_UniqueId,
		Utility::GOPTypeToString(GetGOPType()));
	PLOG_INFO(PREFIX "      Size: %" PRIu32,
		m_UniqueId,
		GetGOPSize());
	PLOG_INFO(PREFIX "      Size Range: %" PRIu32 " - %" PRIu32,
		m_UniqueId,
		GetGOPSizeMin(),
		GetGOPSizeMax());
	PLOG_INFO(PREFIX "      Alignment: %s",
		m_UniqueId,
		IsGOPAlignmentEnabled() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "    Deblocking Filter: %s",
		m_UniqueId,
		IsDeblockingFilterEnabled() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "    Motion Estimation: %s%s",
		m_UniqueId,
		IsMotionEstimationQuarterPixelEnabled() ? (IsMotionEstimationHalfPixelEnabled() ? "Quarter, " : "Quarter") : "",
		IsMotionEstimationHalfPixelEnabled() ? "Half" : "");
	#pragma endregion Picture Control

	#pragma region Intra-Refresh
	PLOG_INFO(PREFIX "  Intra-Refresh:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Mode: %" PRIu32,
		m_UniqueId,
		GetIntraRefreshMode());
	PLOG_INFO(PREFIX "    Frame Number: %" PRIu32,
		m_UniqueId,
		GetIntraRefreshFrameNum());
	#pragma endregion Intra-Refresh

	#pragma region Slicing
	PLOG_INFO(PREFIX "  Slicing:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    Slices Per Frame: %" PRIu32,
		m_UniqueId,
		GetSlicesPerFrame());
	PLOG_INFO(PREFIX "    Control Mode: %s",
		m_UniqueId,
		Utility::SliceControlModeToString(GetSliceControlMode()));
	PLOG_INFO(PREFIX "    Control Size: %" PRIu32,
		m_UniqueId,
		GetSliceControlSize());
	#pragma endregion Slicing

	#pragma region Experimental
	PLOG_INFO(PREFIX "  Experimental:",
		m_UniqueId);
	PLOG_INFO(PREFIX "    QPCBOffset: %" PRIu32,
		m_UniqueId,
		GetQPCBOffset());
	PLOG_INFO(PREFIX "    QPCROffset: %" PRIu32,
		m_UniqueId,
		GetQPCROffset());
	PLOG_INFO(PREFIX "    Input Queue: %" PRIu32,
		m_UniqueId,
		GetInputQueueSize());
	PLOG_INFO(PREFIX "    Low Latency: %s",
		m_UniqueId,
		GetLowLatencyInternal() ? "Enabled" : "Disabled");
	PLOG_INFO(PREFIX "    Ultra Low Latency: %s",
		m_UniqueId,
		GetCommonLowLatencyInternal() ? "Enabled" : "Disabled");
	#pragma endregion Experimental

	//PLOG_INFO(PREFIX "  ");
	//PLOG_INFO(PREFIX "    ");
}
