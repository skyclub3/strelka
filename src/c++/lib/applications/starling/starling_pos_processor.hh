// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Starka
// Copyright (c) 2009-2014 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/sequencing/licenses/>
//

///
/// \author Chris Saunders
///

#pragma once

#include "gvcf_aggregator.hh"
#include "starling_shared.hh"
#include "starling_streams.hh"
#include "site_info_stream.hh"
#include "assembly/assembly_record_processor.hh"

#include "starling_common/starling_pos_processor_base.hh"



///
///
struct starling_pos_processor : public starling_pos_processor_base
{
    typedef starling_pos_processor_base base_t;

    starling_pos_processor(
        const starling_options& opt,
        const starling_deriv_options& dopt,
        const reference_contig_segment& ref,
        const starling_streams& streams);

    /// specify gvcf nocompress status of region
    void
    insert_nocompress_region(
        const known_pos_range2& range);

    void reset();

private:

    void
    clear_pos_annotation(const pos_t pos) override
    {
        _nocompress_regions.removeToPos(pos);
    }

    bool
    is_suspend_read_buffer_clear() override
    {
        return (_gvcfer && _gvcfer->is_phasing_block());
    }

    void
    process_pos_variants_impl(const pos_t pos)
    {
        process_pos_indel_single_sample(pos,0);
        process_pos_snp_single_sample(pos,0);
    }

    void
    process_pos_snp_single_sample(
        const pos_t pos,
        const unsigned sample_no);

    void
    process_pos_snp_single_sample_impl(
        const pos_t pos,
        const unsigned sample_no);

    void
    process_pos_indel_single_sample(
        const pos_t pos,
        const unsigned sample_no);

    void
    write_counts(const pos_range& output_report_range) const override;

    const starling_options& _opt;
    const starling_deriv_options& _dopt;
    const starling_streams& _streams;

    std::shared_ptr<site_info_stream> _gvcfer;

    // a caching term used for gvcf:
    site_info _site_info;

    RegionTracker _nocompress_regions;
    RegionTracker _assembly_regions;
};
