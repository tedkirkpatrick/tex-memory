#!/usr/bin/env python3

import argparse
import csv
import os
import re
import sys
import xml.etree.ElementTree as ET

def parse_args():
    argp = argparse.ArgumentParser(
        description="Parse Catch2 benchmark XML output and output as one CSV line per benchmark")
    argp.add_argument(
        'output_filename',
        help="CSV file to which benchmark entries will be appended")
    argp.add_argument(
        '-i','--input_filename',
        help="XML file containing Catch2 XML output from a benchmark")
    argp.add_argument(
        '--create', action='store_true',
        help='Create new output file with header line. Error if file already exists.')
    args = argp.parse_args()
    if args.create and args.input_filename is not None:
        print("Contradictory arguments: Either --input_filename or --create but not both")
        sys.exit(1)
    if args.create and os.path.exists(args.output_filename):
        print(f"File {args.output_filename} exists. Cannot request --create. Delete file if you want to overwrite it.")
        sys.exit(1)
    if not args.create and args.input_filename is None:
        print("Must specify either --create or --input_filename")
        sys.exit(1)
    if not args.create and not os.path.exists(args.output_filename):
        print(f"Output file {args.output_filename} does not exist. First initialize it with --create.")
        sys.exit(1)
    return args


def process_name(value, result):
    match = re.match(r"([^,]+), *code='([^,]+)', *traversals=(\d+), *optimization='([^']+)', *UTC='([^']+)' *",
                     value)
    assert match is not None, f"Could not parse name {value}"
    result['name'] = match.group(1)
    result['code'] = match.group(2)
    result['traversals'] = int(match.group(3))
    result['optimization'] = match.group(4)
    result['UTC'] = match.group(5)


def process_mean(child, result):
    for att, value in child.attrib.items():
        if att == 'value':
            result['mean_value_ns'] = float(value)
        elif att == 'lowerBound':
            result['mean_lower_bound_ns'] = float(value)
        elif att == 'upperBound':
            result['mean_upper_bound_ns'] = float(value)
        elif att == 'ci':
            result['mean_ci'] = float(value)
        else:
            assert False, f"Unknown mean attribute {att}, {value} in child {child.tag}"


def process_sd(child, result):
    for att, value in child.attrib.items():
        if att == 'value':
            result['sd_value_ns'] = float(value)
        elif att == 'lowerBound':
            result['sd_lower_bound_ns'] = float(value)
        elif att == 'upperBound':
            result['sd_upper_bound_ns'] = float(value)
        elif att == 'ci':
            result['sd_ci'] = float(value)
        else:
            assert False, f"Unknown sd attribute {att}, {value} in child {child.tag}"


def process_outliers(child, result):
    for att, value in child.attrib.items():
        if att == 'variance':
            result['outliers_variance_ns_2'] = float(value)
        elif att == 'lowMild':
            result['outliers_low_mild'] = int(value)
        elif att == 'lowSevere':
            result['outliers_low_severe'] = int(value)
        elif att == 'highMild':
            result['outliers_high_mild'] = int(value)
        elif att == 'highSevere':
            result['outliers_high_severe'] = int(value)
        else:
            assert False, f"Unknown outliers attribute {att}, {value} in child {child.tag}"


def process_result(xml_result):
    result = {}
    for att, value in xml_result.attrib.items():
        if att == 'name':
            process_name(value, result)
        elif att in ['samples', 'resamples', 'iterations']:
            result[att] = int(value)
        elif att == 'estimatedDuration':
            result['estimated_duration_ns'] = float(value)
        else:
            result['clock_resolution_ns'] = float(value)
    for child in xml_result:
        if child.tag == 'mean':
            process_mean(child, result)
        elif child.tag == 'standardDeviation':
            process_sd(child, result)
        elif child.tag == 'outliers':
            process_outliers(child, result)
        else:
            assert False, f"Unknown child tag of BenchmarkResults: {child.tag}"
    return result


def output_result(writer, fields, results):
    row = []
    for field in fields:
        row.append(results[field])
    writer.writerow(row)


def main(args):
    fields = ['name', 'code', 'traversals', 'optimization', 'UTC',
              'samples', 'resamples', 'iterations', 'clock_resolution_ns', 'estimated_duration_ns',
              'mean_value_ns', 'mean_lower_bound_ns', 'mean_upper_bound_ns', 'mean_ci',
              'sd_value_ns', 'sd_lower_bound_ns', 'sd_upper_bound_ns', 'sd_ci',
              'outliers_variance_ns_2', 'outliers_low_severe', 'outliers_low_mild',
              'outliers_high_mild', 'outliers_high_severe']
    if args.create:
        with open(args.output_filename, 'w') as outf:
            writer = csv.writer(outf)
            writer.writerow(fields)
        sys.exit(0)
    tree = ET.parse(args.input_filename)
    root = tree.getroot()
    assert root.tag == 'Catch'
    with open(args.output_filename, 'a') as outf:
        writer = csv.writer(outf)
        for group in root:
            if group.tag != 'Group':
                print(f"Encountered top-level tag not 'Group' ({group.tag}). Ignoring.")
                continue
            for test_case in group:
                if test_case.tag in ['OverallResults', 'OverallResultsCases']:
                    continue
                assert test_case.tag == 'TestCase', f"test_case.tag {test_case.tag}"
                for bm_result in test_case:
                    if bm_result.tag == 'OverallResult':
                        assert bm_result.attrib['success']=="true", f"Overall result success {bm_result.attrib['success']}"
                        continue
                    assert bm_result.tag == 'BenchmarkResults', f"bm_result.tag {bm_result.tag}"
                    results = process_result(bm_result)
                    output_result(writer, fields, results)


if __name__ == '__main__':
    args = parse_args()
    main(args)
