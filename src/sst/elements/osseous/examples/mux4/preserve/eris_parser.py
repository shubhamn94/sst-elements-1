
import sys, os, logging, re as regex
from shutil import copy2

current_token = 0
text = []
files = []

dir = os.getcwd()
for file in os.listdir(dir):
    text.append(dir + '/' + file)
    files.append(file)

outpath  = dir + '/output'
preservepath = dir + '/preserve'
if not os.path.exists(outpath): os.mkdir(outpath)
if not os.path.exists(preservepath): os.mkdir(preservepath)

outfile = outpath + '/eris_parser.log'
if os.path.exists(outfile): os.remove(outfile)

logging.basicConfig(filename=outfile, level=logging.DEBUG,format= '[%(asctime)s.%(msecs)03d] {%(pathname)s:%(lineno)d} %(levelname)-8s - %(message)s',datefmt='%Y-%m-%d %H:%M:%S')
logger = logging.getLogger()
#sys.stderr.write = logger.error
#sys.stdout.write = logger.info

##############################################################################################################################
##############################################################################################################################
class Lexer():

    print('=== START ===')

    for file in os.listdir(dir):
       filepath = os.path.join(dir,file)
       if os.path.isfile(filepath):
           copy2(filepath, dir+'/preserve')
    for file in os.listdir(dir+'/original'):
        copy2(os.path.join(dir+'/original',file), dir)


    position = 0

    for file in text:
        if 'O1.h' in file: # soft code later
            break
    file = open(file).read()

    type = regex.findall(r'typedef struct(.*?)\{', file)[-1].strip()
    print(f'Design detected: {type}')

    # Need to discuss with Shubham a defined naming convention for component.cc component.h and _header.h

    for file in text:
        if 'Makefile.am' in file:
            f = open(file).read()
            makefile_token = f
            break
    '''
    #Find name of design from file names
    for file in files:
        if file[0] == '.': continue
        if 'templ' in file:
            print
        if 'vscode' in file or "store" in file or 'rtlmemm' in file or "Make" in file or 'parser' in file:
            continue
        d = 0
        _files = files
        _files.remove(file)
        file = file.split('.')[0]
        if 'event' in file :
            continue
        for f in files:
            if 'templ' in f:
                print
            f = f.split('.')[0]
            if file == f:
                design = f
                d = 1
                break
        if d == 1:
            #print(f'Design detected: {design}')
            break
        

    #Component and header tokens
    for file in text:
        if design + '.cc' in file:
            f = open(file).read()
            component_token = f
            break

    for file in text:
        if design + '.h' in file:
            f = open(file).read()
            header_token = f
            break
    '''
    f = open('templ.cc').read()
    component_token = f
    f = open('templ.h').read()
    header_token = f

    #I define wrapper code tokens
    wrapper_tokens = []
    for file in text:
        if 'specs' in file:
            f = open(file)
            
            wrapper_file = []
            for l in f:
                wrapper_file.append('\t' + l)
            wrapper_tokens.append(wrapper_file)

            f = open(file).read()
            wrapper_stats  = regex.search(r'SST_ELI_DOCUMENT_STATISTICS\((.*?)\)\n',f, regex.DOTALL).group(1).strip().split('\n') 
            for i in range(len(wrapper_stats)):
                wrapper_stats[i] = regex.search(r'\"(.*?)\"',wrapper_stats[i],).group(1).strip()
            wrapper_tokens.append(wrapper_stats)

            break
    
    #I define port config tokens.
      #Find all lines between section start & end.
    for file in text:
        if 'specs' in file:
            f = open(file).read()
            inp_signals   = regex.search(r'#Input_sig:(.*?)#',                          f, regex.DOTALL).group(1).strip().split('\n') 
            ctrl_signals  = regex.search(r'#Control_sig:(.*?)#',                        f, regex.DOTALL).group(1).strip().split('\n') 
            interfaces    = regex.search(r'#Interface Ports(.*?)#',                     f, regex.DOTALL).group(1).strip().split('\n')  
            int_signals   = regex.search(r'#Interface Signals(.*?)(#|$)',               f, regex.DOTALL).group(1).strip().split('\n')     
            protocol      = regex.search(r'#Config - Master or Slave(.*?)#',            f, regex.DOTALL).group(1).strip().split('\n')  
            mode          = regex.search(r'#Mode(.*?)#',                                f, regex.DOTALL).group(1).strip().split('\n')        
            master_slave  = regex.search(r'#Config - Master or Slave(.*?)#',            f, regex.DOTALL).group(1).strip().split('\n') 
            port_config_tokens = {'inp_signals' : inp_signals, 'ctrl_signals' : ctrl_signals, 'interfaces' : interfaces, 'interface signals' : int_signals,'protocol' : protocol, 'mode' : mode, 'm/s' : master_slave} 
            break
    if ctrl_signals == ['']: ctrl_signals =[]
    if inp_signals == ['']: inp_signals = []

    signal_widths = {}
    uint = 0
    for elt in inp_signals:
        for file in text: 
            if '_O1.h' in file: ########### needs to be set
                f = open(file)
                break
        for l in f:
            if elt in l :
                width = regex.search("\<(.*?)\>",l).group(1) #first occurence needs to be declaration
                signal_widths[elt] = width
                if int(width) < 8:
                    uint += 8
                elif int(width) < 64:
                    uint += 64
                else:
                    uint += 64 * (width // 64 + 1)
                break

    for elt in ctrl_signals:
        for file in text: 
            if '_O1.h' in file: ########### needs to be set
                f = open(file)
                break
        #f = open('VecShiftRegister_O1.h')     ########### needs to be set
        for l in f:
            if elt in l:
                width = regex.search("\<(.*?)\>",l).group(1) #first occurence needs to be declaration
                signal_widths[elt] = width
                break
        
    # I define event tokens.
    event_tokens = []
    for file in text:
        if 'event' in file and '.cc' in file :
            event_name = regex.search(rf'{os.getcwd()}/(.*?event)',file).group(1)
            globals()[str(event_name)] = open(file).read()  # initialize token for xx_event
            event_tokens.append(globals()[str(event_name)])
            break
    eventh_tokens = []
    for file in text:
        if 'event' in file and '.h' in file :
            event_name = regex.search(rf'{os.getcwd()}/(.*?event)',file).group(1)
            globals()[str(event_name)] = open(file).read()  # initialize token for xx_event header
            eventh_tokens.append(globals()[str(event_name)])
            break

    # I create all tokens & give them lexical functionality,
    # for when semantic analyzer is using them.    
    tokens = [event_tokens,eventh_tokens,port_config_tokens, wrapper_tokens, header_token, component_token, makefile_token]
##############################################################################################################################
##############################################################################################################################

##############################################################################################################################
# inject new code 'inj' into existing code 'str' at position 'idx'
def inject(str, inj, idx):
    return str[:idx] + inj + str[idx:]
##############################################################################################################################

##############################################################################################################################
# replace existing in str between position 'sidx' and position 'eidx' with new code 'rep'
def replace(str, rep, sidx, eidx):
    str = str[:sidx] + rep + str[eidx:]
    return str 
##############################################################################################################################


##############################################################################################################################
# Called at program entry point
def parse(text):

    global Tokens
    Tokens = Lexer()
    code_list = parse_code_list(Tokens)
    return code_list
##############################################################################################################################

##############################################################################################################################
def parse_makefile(makefile):

    p = regex.compile('comp_LTLIBRARIES =')
    p = p.finditer(makefile)
    for m in p:
        _p = m.end()
    inj = ' libsst' + Tokens.type +'.la'
    makefile = inject(makefile, inj, _p)

    p = regex.compile('la_SOURCES =')
    p = p.finditer(makefile)
    for m in p:
        _p = m.start()
    inj = 'libsst' + Tokens.type + '_'
    makefile = inject(makefile, inj, _p)

    p = regex.compile(r'la_SOURCES = \\')
    p = p.finditer(makefile)
    for m in p:
        _p = m.end()
    inj = '\n\tsst'+ Tokens.type +'.h \\\n\tsst' + Tokens.type + '.cc \\'
    makefile = inject(makefile, inj, _p)

    p = regex.compile('la_LDFLAGS =')
    p = p.finditer(makefile)
    for m in p:
        _p = m.start()
    inj = 'libsst'+ Tokens.type + '_'
    makefile = inject(makefile, inj, _p)

    p = regex.compile('SST_ELEMENT_SOURCE')
    p = p.finditer(makefile)
    for m in p:
        _p = m.end()
    inj = '  sst' + Tokens.type + '=$(abs_srcdir)'
    makefile = inject(makefile, inj, _p)

    print('Finished creating new makefile ./output/Makefile.am')

    return makefile
##############################################################################################################################

##############################################################################################################################

def parse_component(component):

    # add header file
    p = regex.compile('using namespace')
    p = p.finditer(component)
    for m in p:
        _p = m.start()
        break
    inj = '#include \"sst' + f'{Tokens.type}.h' + '\"\n\n'
    component = inject(component, inj, _p)

    #Insert C model type
    component = regex.sub('TYPE',Tokens.type,component)

    # Create component class
    component = regex.sub('dut','sst'+Tokens.type,component)

    '''
    for elt in Tokens.wrapper_stats: # Add statistics calculation
        w = 0
        match elt:
            case 'read_requests':
                p = regex.compile(r'generateReadRequest\(RtlReadEvent\* rEv\) {')
                p = p.finditer(component)
                for m in p:
                    _p = m.end()
                inj = '\n\n    ++' +  elt + ';'
                w = 1

            case 'write_requests': 
                p = regex.compile(r'generateWriteRequest\(RtlWriteEvent\* wEv\) {')
                p = p.finditer(component)
                for m in p:
                    _p = m.end()
                inj = '\n\n    ++' +  elt + ';'
                w = 1

            case 'split_read_requests':
                p = regex.compile('const uint64_t leftAddr = readAddress;')
                p = p.finditer(component)
                for m in p:
                    _p = m.start()
                inj = '++' + elt + ';\n\n        ' 
                w = 1

            case 'split_write_requests': 
                p = regex.compile('const uint64_t leftAddr = writeAddress;')
                p = p.finditer(component)
                for m in p:
                    _p = m.start()
                inj = '++' + elt + ';\n\n        '
                w = 1

            #case 'flush_requests':
            #    print()

            #case 'fence_requests': 
            #    print()
        if w == 1:
            component = inject(component, inj, _p)
        else: print('WARNING: The ' + elt + ' statistic was not added.')
    '''
    print('Finished creating new rtl component file ./output/sst' + Tokens.type + '.cc')

    return component

##############################################################################################################################

##############################################################################################################################
def parse_header(header):

    # Declare C model
    header = regex.sub('TYPE',Tokens.type,header)


    for elt in text:
        if 'O1.h' in elt:
            p = regex.compile('namespace SST')
            p = p.finditer(header)
            for m in p:
                _p = m.start()
                break
            inj = '#include \"' + elt.split('/')[-1] + '\"\n\n'
            header = inject(header,inj,_p)
            break


    if len(regex.findall('private',header)) != 1: raise Exception(f'Cannot determine \'private:\' keyword location in header. {header}')
    else: 
        p = regex.compile('private:')
        p = p.finditer(header)
        for m in p:
            _p = m.start()-1
            for elt in Tokens.wrapper_file:
                if 'Start header' in elt:
                    ws = Tokens.wrapper_file.index(elt) +1
            for elt in Tokens.wrapper_file:
                if 'End header' in elt:
                    we = Tokens.wrapper_file.index(elt)
            inj = '\n' + ''.join(Tokens.wrapper_file[ws:we])
            header = inject(header, inj ,_p)
        header = inject(header, '\n\n', _p)

        p = regex.compile('private:')
        p = p.finditer(header)
        for m in p:
            _p = m.end()+1
            header = inject(header, '\n', _p)
            _p+=1
            for elt in Tokens.wrapper_stats: # Declare statistics
                inj = '    Statistic<uint64_t>* ' + elt + ';\n'
                header = inject(header, inj, _p)
                _p += len(inj)
            header = inject(header, '\n', _p)
    header = regex.sub('dut','sst'+Tokens.type,header)

    print('Finished creating new rtl header file ./output/sst' + Tokens.type + '.h')

    return header
##############################################################################################################################

##############################################################################################################################
def parse_events(events):
    for event in events:
        if len(regex.findall('::input_sigs',event)) != 1: raise Exception('Cannot determine ::input_sigs scope in events.')
        else: # Add input signals to RTL Events
            p = regex.compile(r'::input_sigs\(')
            p = p.finditer(event)
            for m in p:
                _p = m.end()
                _event = event[_p:]
                #type = regex.search(r'\((.*?)\)', _event).group(1).split(' ')[0]
                dut  = regex.search(r'\((.*?)\)', _event).group(1).split(' ')[1]
                inj = Tokens.type
                event = inject(event, inj ,_p)

                _p = regex.compile(rf'::input_sigs\({Tokens.type}')
                _p = _p.finditer(event)
                for _m in _p:
                    __p =  _m.end()
                    __p = __p + 1 + len(dut) + 3 + 2
                    inj = f'\n    {dut}->reset = UInt<1>(1);\n'
                    event = inject(event, inj ,__p)
                    __p += len(inj)
                    
                    i = 0
                    for elt in Tokens.inp_signals:
                        w = Tokens.signal_widths[elt]
                        n = r'\n'
                        if i == 0:
                            inj = f'\n    UInt<{w}>* rtl_inp_ptr = (UInt<{w}>*)inp_ptr;\n    {dut}->{elt} = rtl_inp_ptr[{i}];\n    fprintf(stderr, "{n}input_sigs: %" PRIu8, {dut}->{elt});\n    ++rtl_inp_ptr;\n'
                        else:
                            inj = inj = f'\n    rtl_inp_ptr = (UInt<{w}>*)inp_ptr;\n    {dut}->{elt} = rtl_inp_ptr[{i}];\n    fprintf(stderr, "{n}input_sigs: %" PRIu8, {dut}->{elt});\n    ++rtl_inp_ptr;\n'
                        event = inject(event, inj, __p)
                        __p += len(inj)
                        i+=1

                    inj = f'\n    {dut}->reset = UInt<1>(0);\n'
                    event = inject(event, inj ,__p)

        if len(regex.findall('::control_sigs',event)) != 1: raise Exception('Cannot determine ::ctrl_sigs scope in events.')
        else: # Add ctrl signals to RTL Events
            p = regex.compile(r'::control_sigs\(')
            p = p.finditer(event)
            for m in p:
                _p = m.end()
                inj = Tokens.type
                event = inject(event, inj ,_p)

            _p = regex.compile(rf'::control_sigs\({Tokens.type}')
            _p = _p.finditer(event)
            for _m in _p:
                __p =  _m.end()
                __p = __p + 1 + len(dut) + 3 + 2
                inj = f'\n    {dut}->reset = UInt<1>(1);\n'
                event = inject(event, inj ,__p)
                __p += len(inj)
                i = 0
                for elt in Tokens.ctrl_signals:
                    w = Lexer.signal_widths[elt]
                    n = r'\n'
                    if i == 0 :
                        inj = f'\n    UInt<{w}>* rtl_ctrl_ptr = (UInt<{w}>*)ctrl_ptr;\n    {dut}->{elt} = rtl_ctrl_ptr[{i}];\n    fprintf(stderr, "{n}ctrl_sigs: %" PRIu8, {dut}->{elt});\n    ++rtl_ctrl_ptr;\n'
                    else:
                         inj = f'\n    rtl_ctrl_ptr = (UInt<{w}>*)ctrl_ptr;\n    {dut}->{elt} = rtl_ctrl_ptr[{i}];\n    fprintf(stderr, "{n}ctrl_sigs: %" PRIu8, {dut}->{elt});\n    ++rtl_ctrl_ptr;\n'
                    event = inject(event, inj, __p)
                    __p += len(inj)
                    i+=1
                inj = f'\n    {dut}->reset = UInt<1>(0);\n'
                event = inject(event, inj ,__p)

        if len(regex.findall('RTLEvent::UpdateRtlSignals',event)) != 1: raise Exception('Cannot determine RTLEvent::UpdateRtlSignals function in events.')
        else: # Add ctrl signals to RTL Events
            p = regex.compile(r'RTLEvent::UpdateRtlSignals\(void \*update_data, ')
            p = p.finditer(event)
            for m in p:
                _p = m.end()
                inj = Tokens.type
                event = inject(event, inj ,_p)


        p = regex.compile(r'UInt<>\* rtl_inp_ptr = \(UInt<>\*\)inp_ptr;')
        p = p.finditer(event)
        for m in p:
            s = m.start()
            e = m.end()
            rep = f'UInt<{Tokens.uint}>* rtl_inp_ptr = (UInt<{Tokens.uint}>*)inp_ptr;'
            event = replace(event, rep, s , e)

    print('Finished creating new rtl event file ./output/' + Tokens.event_name + '.cc')

    return event
##############################################################################################################################

##############################################################################################################################
def parse_hevents(hevents):
    for hevent in hevents:
        # add header file
        for elt in text:
            if '_O1.h' in elt:                      # soft code this once file naming convention is defined
                p = regex.compile('namespace SST')
                p = p.finditer(hevent)
                for m in p:
                     _p = m.start()
                     break
                inj = '#include \"' + elt.split('/')[-1] + '\"\n\n'
                hevent = inject(hevent, inj, _p)
                break


        if len(regex.findall('input_sigs',hevent)) != 1: raise Exception('Cannot determine input_sigs location in event header.')
        else:
            p = regex.compile(r'input_sigs\(')
            p = p.finditer(hevent)
            for m in p:
                _p = m.end()
                inj = Tokens.type
                hevent = inject(hevent, inj ,_p)
        if len(regex.findall('control_sigs',hevent)) != 1: raise Exception('Cannot determine input_sigs location in event header.')
        else:
            p = regex.compile(r'control_sigs\(')
            p = p.finditer(hevent)
            for m in p:
                _p = m.end()
                inj = Tokens.type
                hevent = inject(hevent, inj ,_p)
        if len(regex.findall('void UpdateRtlSignals',hevent)) != 1: raise Exception('Cannot determine UpdateRtlSignals function location in event header.')
        else:
            p = regex.compile(r'void UpdateRtlSignals\(void\*, ')
            p = p.finditer(hevent)
            for m in p:
                _p = m.end()
                inj = Tokens.type
                hevent = inject(hevent, inj ,_p)
    print('Finished creating new rtl event header file ./output/' + Tokens.event_name + '.h')

    return(hevent)
##############################################################################################################################

##############################################################################################################################
def parse_code(current_token):
    # Tokens that are output (edited code)
    # get their own parse. Other tokens get used
    # in their parsing/editing via peek().

    if current_token is Tokens.port_config_tokens:
        return None
    elif current_token is Tokens.wrapper_tokens:
        return None
    elif current_token is Tokens.event_tokens:
        return { 'rtlevent.cc' : parse_events(current_token) } #eventually this filename will need to be created dynamically, ie other events
    elif current_token is Tokens.eventh_tokens:
        return { 'rtlevent.h' : parse_hevents(current_token) } #eventually this filename will need to be created dynamically, ie other events
    elif current_token is Tokens.header_token:
        return { f'{Tokens.type}.h' :parse_header(current_token) }
    elif current_token is Tokens.component_token:
        return { f'{Tokens.type}.cc': parse_component(current_token) }
    elif current_token is Tokens.makefile_token:
        return { 'Makefile.am': parse_makefile(current_token) }
    else:
        return None
##############################################################################################################################

##############################################################################################################################
def main():
    
    code_list = parse(text)
    for elt in code_list:
        for k,v, in elt.items():
            if k == f'{Tokens.type}.cc': k = 'sst' + k
            if k == f'{Tokens.type}.h' : k = 'sst' + k
            f = open(os.path.join(dir +'/output', k), 'w')
            f.write(v)
            f.close()
            os.listdir(os.path.join(dir +'/output'))
            

    for file in os.listdir(dir+'/output'):
        copy2(os.path.join(dir+'/output',file), dir)
    print('=== END ===')
##############################################################################################################################

##############################################################################################################################
def parse_code_list(Tokens):
    code_list = []
    for idx,current_token in enumerate(Tokens.tokens):

        code = parse_code(current_token)
        if code is not None: code_list.append(code)

    return code_list
##############################################################################################################################

main()

