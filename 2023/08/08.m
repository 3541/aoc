#import <Foundation/Foundation.h>
#import <stddef.h>

@interface Branch : NSObject {
@public
    NSString const* left;
    NSString const* right;
}
- (id)init: (NSString const*)left : (NSString const*)right;
@end

@implementation Branch
- (id)init: (NSString const*)l : (NSString const*)r {
    self = [super init];
    if (self) {
        left = l;
        right = r;
    }

    return self;
}
@end

size_t gcd(size_t a, size_t b) {
    if (a == 0)
        return b;

    return gcd(b % a, a);
}

size_t lcm(size_t a, size_t b) {
    return (a * b) / gcd(a, b);
}

int main(void) {
    @autoreleasepool {
        NSString const* input =
            [NSString stringWithContentsOfFile:[[NSProcessInfo processInfo] arguments][1]];
        NSArray const* lines = [input componentsSeparatedByString:@"\n"];
        NSString const* path = lines[0];

        NSMutableDictionary* graph = [[NSMutableDictionary alloc] init];
        for (size_t i = 1; i < [lines count]; ++i) {
            NSString const* line = lines[i];
            if ([line length] == 0)
                continue;

            NSArray const* parts = [line componentsSeparatedByString:@" = "];
            NSArray const* branches = [parts[1] componentsSeparatedByString:@", "];
            [graph setObject:[[Branch alloc] init:[branches[0] substringFromIndex:1]
                                                 :[branches[1] substringToIndex:3]] forKey:parts[0]];
        }

        if (graph[@"AAA"]) {
            NSString const* current = @"AAA";
            size_t steps = 0;
            for (size_t i = 0; ![current isEqualToString:@"ZZZ"]; i = (i + 1) % [path length], ++steps) {
                unichar const c = [path characterAtIndex:i];
                Branch const* b = graph[current];

                switch (c) {
                case 'R':
                    current = b->right;
                    break;
                case 'L':
                    current = b->left;
                    break;
                default:
                    continue;
                }
            }

            NSLog(@"%zu", steps);
        }

        NSMutableArray* positions = [[NSMutableArray alloc] init];
        for (NSString const* p in [graph allKeys]) {
            if ([p characterAtIndex:2] == 'A') {
                [positions addObject:p];
            }
        }

        NSMutableArray* counts = [[NSMutableArray alloc] init];
        for (NSString const* p in positions) {
            NSString const* current = p;
            size_t steps = 0;
            for (size_t i = 0; [current characterAtIndex:2] != 'Z'; i = (i + 1) % [path length], ++steps) {
                unichar const c = [path characterAtIndex:i];
                Branch const* b = graph[current];

                switch (c) {
                case 'R':
                    current = b->right;
                    break;
                case 'L':
                    current = b->left;
                    break;
                default:
                    continue;
                }
            }

            [counts addObject:[NSNumber numberWithUnsignedInteger:steps]];
        }

        size_t l = 1;
        for (NSNumber const* n in counts) {
            l = lcm(l, [n unsignedIntegerValue]);
        }

        NSLog(@"%zu", l);
    }
}
